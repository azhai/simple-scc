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
#include "../inc/ar.h"
#include "nm.h"

char *argv0;
static int radix = 16;
static int Pflag;
static int Aflag;
static int vflag;
static int gflag;
static int uflag;
static int arflag;

int
object(char *fname, FILE *fp)
{
	extern struct objfile formats[];
	struct objfile *p;

	for (p = formats; p->probe && (*p->probe)(fp); ++p)
		;
	if (!p->probe)
		return 0;
	(*p->nm)(fname, fp);
	return 1;
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
	fseek(fp, sizeof(struct ar_hdr), SEEK_CUR);

	while (fread(&hdr, sizeof(hdr), 1, fp) == 1) {
		pos = ftell(fp);
		if (strncmp(hdr.ar_fmag, ARFMAG, strlen(ARFMAG)))
			goto corrupted;

		siz = 0;
		sscanf(hdr.ar_size, "%10ld", &siz);
		if (siz == 0)
			goto corrupted;

		if (siz & 1)
			siz++;
		if (pos == -1 || pos > LONG_MAX - siz) {
			fprintf(stderr,
			        "nm: %s: overflow in size of archive\n",
			        fname);
			exit(1);
		}
		pos += siz;

		getfname(&hdr, member);
		if (!object(member, fp)) {
			fprintf(stderr,
			        "nm: skipping member %s in archive %s\n",
			        member, fname);
		}
		fseek(fp, pos, SEEK_SET);
	}
	return;

corrupted:
	fprintf(stderr, "nm: %s: corrupted archive\n", fname);
	exit(1);
}

static int
archive(char *fname, FILE *fp)
{
	char magic[SARMAG];
	fpos_t pos;

	fgetpos(fp, &pos);
	fread(magic, SARMAG, 1, fp);
	fsetpos(fp, &pos);

	if (ferror(fp)) {
		perror("nm");
		exit(1);
	}
	if (strncmp(magic, ARMAG, SARMAG) != 0)
		return 0;
	ar(fname, fp);
	return 1;
}

void
print(char *file, char *member, struct symbol *sym)
{
	char *fmt;
	int type = sym->type;

	if (uflag && type != 'U')
		return;
	if (gflag && type != 'A' && type != 'B' && type != 'D')
		return;

	if (Aflag)
		printf((arflag) ? "%s[%s]: " : "%s: ", file, member);
	if (Pflag) {
		printf("%s %c", sym->name, sym->type);
		if (type != 'U') {
			if (radix == 8)
				fmt = "%llo %llo";
			else if (radix == 10)
				fmt = "%llu %llu";
			else
				fmt = "%llx %llx";
			printf(fmt, sym->off, sym->size);
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
		printf(fmt, sym->off);
		printf(" %c %s", sym->type, sym->name);
	}
	putchar('\n');
}

static int
cmp(const void *p1, const void *p2)
{
	const struct symbol *s1 = p1, *s2 = p2;

	if (vflag)
		return s1->off - s2->off;
	else
		return strcmp(s1->name, s2->name);
}

void
printsyms(char *file, char *member, struct symbol *syms, size_t nsyms)
{
	qsort(syms, nsyms, sizeof(*syms), cmp);

	while (nsyms--)
		print(file, member, syms++);
}

void
doit(char *fname)
{
	FILE *fp;

	arflag = 0;
	if ((fp = fopen(fname, "rb")) == NULL) {
		perror("nm");
		exit(1);
	}

	if (!object(fname, fp) && !archive(fname, fp))
		fprintf(stderr, "nm: %s: File format not recognized\n", fname);

	if (ferror(fp) || fclose(fp) == EOF) {
		perror("nm");
		exit(1);
	}
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
