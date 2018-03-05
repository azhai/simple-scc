static char sccsid[] = "@(#) ./nm/main.c";

#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../inc/arg.h"
#include "../inc/scc.h"
#include "../inc/myro.h"
#include "../inc/ar.h"

char *argv0;
char *strings;
static int radix = 16;
static int Pflag;
static int Aflag;
static int vflag;
static int gflag;
static int uflag;
static int arflag;

static int
object(char *fname, FILE *fp)
{
	char magic[MYROMAGIC_SIZ];
	fpos_t pos;

	fgetpos(fp, &pos);
	fread(magic, sizeof(magic), 1, fp);
	fsetpos(fp, &pos);

	if (!ferror(fp) && !strncmp(magic, MYROMAGIC, MYROMAGIC_SIZ))
		return 1;
	return 0;
}

static int
archive(char *fname, FILE *fp)
{
	char magic[SARMAG];
	fpos_t pos;

	fgetpos(fp, &pos);
	fread(magic, SARMAG, 1, fp);
	fsetpos(fp, &pos);

	if (!ferror(fp) && !strncmp(magic, ARMAG, SARMAG))
		return 1;
	return 0;
}

static int
cmp(const void *p1, const void *p2)
{
	const struct myrosym *s1 = p1, *s2 = p2;

	if (vflag)
		return s1->offset - s2->offset;
	else
		return strcmp(strings + s1->name, strings + s2->name);
}

static int
typeof(struct myrosym *sym)
{
	int t, flags = sym->flags;

	switch (sym->section) {
	case MYRO_TEXT:
		t = 't';
		break;
	case MYRO_DATA:
		t = 'd';
		break;
	case MYRO_BSS:
		t = (flags & MYROSYM_COMMON) ? 'c' : 'b';
		break;
	case MYRO_ABS:
		t = 'a';
		break;
	default:
		t = (flags & MYROSYM_UNDEF) ? 'u' : '?';
		break;
	}
	if (flags & MYROSYM_ABS)
		t = 'a';
	if (flags & MYROSYM_EXTERN)
		t = tolower(t);
	return t;
}

static void
print(char *file, char *member, struct myrosym *sym)
{
	char *fmt, *name = strings + sym->name;
	int type = typeof(sym);

	if (uflag && type != 'U')
		return;
	if (gflag && type != 'A' && type != 'B' && type != 'D')
		return;

	if (Aflag)
		printf((arflag) ? "%s[%s]: " : "%s: ", file, member);
	if (Pflag) {
		printf("%s %c", name, type);
		if (type != 'U') {
			if (radix == 8)
				fmt = "%llo %llo";
			else if (radix == 10)
				fmt = "%llu %llu";
			else
				fmt = "%llx %llx";
			printf(fmt, sym->offset, sym->len);
		}
	} else {
		if (type == 'U')
			fmt = "                ";
		else if (radix == 8)
			fmt = "%016.16llo";
		else if (radix == 8)
			fmt = "%016.16lld";
		else
			fmt = "%016.16llx";
		printf(fmt, sym->offset);
		printf(" %c %s", type, name);
	}
	putchar('\n');
}

static void
nm(char *fname, char *member, FILE *fp)
{
	struct myrohdr hdr;
	struct myrosym *syms = NULL;
	size_t n, i;
	long off;

	strings = NULL;
	if (rdmyrohdr(fp, &hdr) < 0) {
		fprintf(stderr, "nm: %s: incorrect header\n", member);
		return;
	}

	n = hdr.symsize / MYROSYM_SIZ;
	if (n == 0) {
		fprintf(stderr, "nm: %s: no name list\n", member);
		return;
	}
	if (n > SIZE_MAX / sizeof(struct myrosym) ||
	    hdr.symsize / MYROSYM_SIZ > SIZE_MAX ||
	    hdr.strsize > SIZE_MAX) {
		goto offset_overflow;
	}

	syms = xmalloc(n * sizeof(struct myrosym));
	strings = xmalloc(hdr.strsize);
	fread(strings, hdr.strsize, 1, fp);
	if (feof(fp))
		goto free_arrays;
	if ((off = ftell(fp)) < 0)
		return;
	if (off > LONG_MAX - hdr.secsize)
		goto offset_overflow;
	off += hdr.secsize;

	if (fseek(fp, off, SEEK_SET) < 0)
		goto free_arrays;

	for (i = 0; i < n; ++i) {
		if (rdmyrosym(fp, &syms[i]) < 0)
			goto symbol_error;
		if (syms[i].name >= hdr.strsize)
			goto offset_overflow;
	}
	qsort(syms, n, sizeof(*syms), cmp);
	for (i = 0; i < n; ++i)
		print(fname, member, &syms[i]);


free_arrays:
	free(syms);
	free(strings);
	return;

symbol_error:
	fprintf(stderr, "nm: %s: error reading symbols\n", fname);
	goto free_arrays;

offset_overflow:
	fprintf(stderr, "nm: %s: overflow in headers of archive\n",
		fname);
	goto free_arrays;
}

static void
ar(char *fname, FILE *fp)
{
	struct ar_hdr hdr;
	long pos, siz;

	arflag = 1;
	fseek(fp, sizeof(struct ar_hdr), SEEK_CUR);

	while (fread(&hdr, sizeof(hdr), 1, fp) == 1) {
		pos = ftell(fp);
		sscanf(hdr.ar_size, "%10ld", &siz);
		if (pos == -1 || pos > LONG_MAX - siz) {
			fprintf(stderr,
			        "nm: %s: overflow in size of archive\n",
			        fname);
			return;
		}
		pos += siz;
		if (siz & 1)
			++pos;

		if (object(fname, fp)) {
			nm(fname, hdr.ar_name, fp);
		} else {
			fprintf(stderr,
			        "nm: skipping member %s in archive %s\n",
			        hdr.ar_name, fname);
		}
		fseek(fp, pos, SEEK_SET);
	}
}

void
doit(char *fname)
{
	FILE *fp;

	arflag = 0;
	if ((fp = fopen(fname, "rb")) == NULL)
		goto file_error;

	if (object(fname, fp))
		nm(fname, fname, fp);
	else if (archive(fname, fp))
		ar(fname, fp);
	else
		fprintf(stderr, "nm: %s: File format not recognized\n", fname);

	if (fclose(fp) != EOF)
		return;

file_error:
	die("nm: %s: %s", fname, strerror(errno));
}

void
usage(void)
{
	fputs("nm [-APv][ -g| -u][-t format] [file...]\n", stderr);
	exit(1);
}

int
main(int argc, char *argv[])
{
	char *t;

	ARGBEGIN {
	case 'P':
		Pflag = 1;
		break;
	case 'A':
		Aflag = 1;
		break;
	case 'g':
		gflag = 1;
		break;
	case 'u':
		uflag = 1;
		break;
	case 'v':
		vflag = 1;
		break;
	case 't':
		t = EARGF(usage());
		if (!strcmp(t, "o"))
			radix = 8;
		else if (!strcmp(t, "d"))
			radix = 10;
		else if (!strcmp(t, "x"))
			radix = 16;
		else
			usage();
		break;
	default:
		usage();
	} ARGEND

	if (argc == 0) {
		doit("a.out");
	} else {
		for ( ; *argv; ++argv)
			doit(*argv);
	}

	return 0;
}
