static char sccsid[] = "@(#) ./nm/main.c";

#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <scc/arg.h>
#include <scc/scc.h>
#include <scc/ar.h>
#include "nm.h"

char *argv0;
static int radix = 16;
static int Pflag;
static int Aflag;
static int vflag;
static int gflag;
static int uflag;
static int arflag;

static int
object(char *fname, char *member, FILE *fp)
{
	extern struct objfile *formats[];
	struct objfile **p, *obj;
	void *data;

	for (p = formats; *p; ++p) {
		obj = *p;
		if ((*obj->probe)(fname, member, fp))
			break;
	}
	if (*p == NULL)
		return 0;
	return (*obj->nm)(fname, member, fp);
}

static char *
getfname(struct ar_hdr *hdr, char *dst)
{
	char *p;
	int i;

	memcpy(dst, hdr->ar_name, SARNAM);
	dst[SARNAM] = '\0';

	for (i = SARNAM-1; i >= 0; i--) {
		if (dst[i] != ' ' && dst[i] != '/')
			break;
		dst[i] = '\0';
	}
	return dst;
}

static void
ar(char *fname, FILE *fp)
{
	struct ar_hdr hdr;
	long pos, siz;
	char member[SARNAM+1];

	arflag = 1;
	if (fseek(fp, SARMAG, SEEK_SET) == EOF)
		goto file_error;

	while (fread(&hdr, sizeof(hdr), 1, fp) == 1) {
		pos = ftell(fp);
		if (strncmp(hdr.ar_fmag, ARFMAG, sizeof(hdr.ar_fmag)))
			goto corrupted;

		siz = 0;
		sscanf(hdr.ar_size, "%10ld", &siz);
		if (siz == 0)
			goto corrupted;

		if (siz & 1)
			siz++;
		if (pos == -1 || pos > LONG_MAX - siz)
			die("nm: %s: overflow in size of archive", fname);
		pos += siz;

		getfname(&hdr, member);
		if (!object(fname, member, fp)) {
			fprintf(stderr,
			        "nm: skipping member %s in archive %s\n",
			        member, fname);
		}
		if (fseek(fp, pos, SEEK_SET) == EOF)
			goto file_error;
	}
	if (ferror(fp))
		goto file_error;
	return;

corrupted:
	die("nm: %s: corrupted archive", fname);
file_error:
	die("nm: %s: %s", fname, strerror(errno));
}

static int
archive(char *fname, FILE *fp)
{
	char magic[SARMAG];
	fpos_t pos;

	fgetpos(fp, &pos);
	fread(magic, SARMAG, 1, fp);
	fsetpos(fp, &pos);

	if (ferror(fp))
		die("nm: %s: %s", fname, strerror(errno));
	if (strncmp(magic, ARMAG, SARMAG) != 0)
		return 0;

	ar(fname, fp);
	return 1;
}

static void
printsym(char *file, char *member, struct symbol *sym)
{
	char *fmt;
	int type = sym->type;

	if (type == '?')
		return;

	if (uflag && type != 'U')
		return;

	if (gflag && !isupper(type))
		return;

	if (Aflag)
		printf((arflag) ? "%s[%s]: " : "%s: ", file, member);
	if (Pflag) {
		printf("%s %c", sym->name, sym->type);
		if (type != 'U') {
			if (radix == 8)
				fmt = " %016.16llo %lo";
			else if (radix == 10)
				fmt = " %016.16llu %lu";
			else
				fmt = " %016.16llx %lx";
			printf(fmt, sym->value, sym->size);
		}
	} else {
		if (type == 'U')
			fmt = "                ";
		else if (radix == 8)
			fmt = "%016.16llo";
		else if (radix == 10)
			fmt = "%016.16lld";
		else
			fmt = "%016.16llx";
		printf(fmt, sym->value);
		printf(" %c %s", sym->type, sym->name);
	}
	putchar('\n');
}

static int
cmp(const void *p1, const void *p2)
{
	const struct symbol *s1 = p1, *s2 = p2;

	if (vflag) {
		if (s1->value > s2->value)
			return 1;
		if (s1->value < s2->value)
			return -1;
		if (s1->type == 'U' && s2->type == 'U')
			return 0;
		if (s1->type == 'U')
			return -1;
		if (s2->type == 'U')
			return 1;
		return 0;
	} else {
		return strcmp(s1->name, s2->name);
	}
}

void
printsyms(char *file, char *member, struct symbol *syms, size_t nsyms)
{
	qsort(syms, nsyms, sizeof(*syms), cmp);

	while (nsyms--)
		printsym(file, member, syms++);
}

static void
doit(char *fname)
{
	FILE *fp;

	arflag = 0;

	if ((fp = fopen(fname, "rb")) == NULL)
		die("nm: %s: %s", fname, strerror(errno));

	if (!object(fname, fname, fp) && !archive(fname, fp))
		die("nm: %s: File format not recognized", fname);

	if (ferror(fp))
		die("nm: %s: %s", fname, strerror(errno));

	fclose(fp);
}

static void
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

	fflush(stdout);
	if (ferror(stdout))
		die("nm: error writing in output");

	return 0;
}
