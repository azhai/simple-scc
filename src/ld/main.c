static char sccsid[] = "@(#) ./ld/main.c";

#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <scc/scc.h>
#include <scc/ar.h>
#include <scc/syslibs.h>
#include "ld.h"

char *output = "a.out", *entry = "start", *datasiz;
int pass;
int sflag;		/* discard all the symbols */
int xflag;		/* discard local symbols */
int Xflag;		/* discard locals starting with 'L' */
int rflag;		/* preserve relocation bits */
int dflag;		/* define common even with rflag */
int gflag;              /* preserve debug symbols */

static int done;

void
redefined(Obj *obj, Symbol *sym)
{
	/* TODO: add infotmation about where it is defined */
	fprintf(stderr,
		"ld: %s: redifinition of symbol '%s'\n",
		obj->fname, sym->name);
}

void
corrupted(char *fname, char *member)
{
	char *fmt;

	fmt = (member) ?
		"ld: %s(%s): corrupted file\n" : "ld: %s: corrupted file\n";
	fprintf(stderr, fmt, fname, member);
	exit(EXIT_FAILURE);
}

void
outmem(void)
{
	fputs("ld: out of memory\n", stderr);
	exit(EXIT_FAILURE);
}

static void
cleanup(void)
{
	if (!done)
		remove(output);
}

static int
object(char *fname, char *member, FILE *fp)
{
	Obj *obj;

	obj = probe(fname, member, fp);
	if (!obj)
		return 0;
	load(obj);

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

	if (fseek(fp, SARMAG, SEEK_SET) == EOF)
		goto file_error;

	while (fread(&hdr, sizeof(hdr), 1, fp) == 1) {
		if (strncmp(hdr.ar_fmag, ARFMAG, sizeof(hdr.ar_fmag)))
			corrupted(fname, NULL);

		siz = 0;
		sscanf(hdr.ar_size, "%10ld", &siz);
		if (siz & 1)
			siz++;
		if (siz == 0)
			corrupted(fname, NULL);

		pos = ftell(fp);
		if (pos == -1 || pos > LONG_MAX - siz) {
			fprintf(stderr,
			        "ld: %s(%s): overflow in size of archive",
			         fname, member);
			exit(EXIT_FAILURE);
		}
		pos += siz;

		getfname(&hdr, member);
		object(fname, member, fp);
		if (fseek(fp, pos, SEEK_SET) == EOF)
			break;
	}

file_error:
	if (ferror(fp)) {
		fprintf(stderr, "ld: %s: %s\n", fname, strerror(errno));
		exit(EXIT_FAILURE);
	}
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
		return 0;
	if (strncmp(magic, ARMAG, SARMAG) != 0)
		return 0;

	ar(fname, fp);
	return 1;
}

static void
pass1(int argc, char *argv[])
{
	FILE *fp;
	char *s;

	while ((s = *argv++) != NULL) {
		if ((fp = fopen(s, "rb")) == NULL) {
			fprintf(stderr, "ld: %s: %s\n", s, strerror(errno));
			exit(EXIT_FAILURE);
		}
		if (!object(s, NULL, fp) && !archive(s, fp)) {
			fprintf(stderr, "ld: %s: File format not recognized\n", s);
			exit(EXIT_FAILURE);
		}
		fclose(fp);
	}
}

static void
pass2(int argc, char *argv[])
{
	FILE *fp;

	if ((fp = fopen(output, "wb")) != NULL) {
		writeout(fp);
		if (fclose(fp) != EOF)
			return;
	}

	fprintf(stderr, "ld: %s: %s\n", output, strerror(errno));
	exit(EXIT_FAILURE);
}

static void
usage(void)
{
	fputs("usage: ld [options] file ...\n", stderr);
	exit(1);
}

static void
Lpath(char *path)
{
	char **bp;

	for (bp = syslibs; bp < &syslibs[MAX_LIB_PATHS] && *bp; ++bp)
		;
	if (bp == &syslibs[MAX_LIB_PATHS]) {
		fputs("ld: too many -L options\n", stderr);
		exit(1);
	}
	*bp = path;
}

int
main(int argc, char *argv[])
{
	char *cp, **p;

	for (--argc; *++argv; --argc) {
		if (argv[0][0] != '-' || argv[0][1] == 'l')
			break;
		if (argv[0][1] == '-') {
			--argc, ++argv;
			break;
		}
		for (cp = &argv[0][1]; *cp; ++cp) {
			switch (*cp) {
			case 's':
				sflag = 1;
				break;
			case 'x':
				xflag = 1;
				break;
			case 'X':
				Xflag = 1;
				break;
			case 'r':
				rflag = 1;
				break;
			case 'd':
				dflag = 1;
				break;
			case 'i':
			case 'n':
				/* TODO */
				break;
			case 'L':
				if (argc == 0)
					goto usage;
				++argv, --argc;
				Lpath(*argv);
				break;
			case 'u':
				if (argc == 0)
					goto usage;
				++argv, --argc;
				lookup(*argv, INSTALL);
				break;
			case 'o':
				if (argc == 0)
					goto usage;
				++argv, --argc;
				output = *argv;
				break;
			case 'e':
				if (argc == 0)
					goto usage;
				++argv, --argc;
				entry = *argv;
				break;
			case 'D':
				if (argc == 0)
					goto usage;
				++argv, --argc;
				datasiz = *argv;
				break;
			default:
			usage:
				usage();
			}
		}
	}

	if (argc == 0)
		usage();

	atexit(cleanup);

	pass1(argc, argv);
	pass2(argc, argv);

	done = 1;

	return 0;
}
