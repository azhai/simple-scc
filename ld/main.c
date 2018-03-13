static char sccsid[] = "@(#) ./ld/main.c";

#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../inc/arg.h"
#include "../inc/scc.h"
#include "../inc/ar.h"
#include "ld.h"

char *argv0;
int pass;

static int
object(char *fname, char *member, FILE *fp)
{
	extern struct objfile *formats[];
	struct objfile **p, *obj;
	void *data;
	int (*fun)(char *, char *, FILE *);

	for (p = formats; *p; ++p) {
		obj = *p;
		if ((*obj->probe)(fname, member, fp))
			break;
	}
	if (*p == NULL)
		return 0;

	fun = (pass == 1) ? obj->pass1 : obj->pass2;
	return (*fun)(fname, member, fp);
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
			die("ld: %s: overflow in size of archive", fname);
		pos += siz;

		getfname(&hdr, member);
		object(fname, member, fp);
		if (fseek(fp, pos, SEEK_SET) == EOF)
			goto file_error;
	}

	if (ferror(fp))
		goto file_error;
	return;

corrupted:
	die("ld: %s: corrupted archive", fname);
file_error:
	die("ld: %s: %s", fname, strerror(errno));
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
		die("ld: %s: %s", fname, strerror(errno));
	if (strncmp(magic, ARMAG, SARMAG) != 0)
		return 0;

	ar(fname, fp);
	return 1;
}

static void
process(char *fname)
{
	FILE *fp;

	if ((fp = fopen(fname, "rb")) == NULL)
		die("ld: %s: %s", fname, strerror(errno));

	if (!object(fname, fname, fp) && !archive(fname, fp))
		die("ld: %s: File format not recognized", fname);

	if (ferror(fp))
		die("ld: %s: %s", fname, strerror(errno));

	fclose(fp);
}

static void
pass1(struct items *list)
{
	unsigned i;

	pass = 1;
	for (i = 0; i < list->n; ++i)
		process(list->s[i]);
}

static void
pass2(struct items *list)
{
	unsigned i;

	pass = 2;
	for (i = 0; i < list->n; ++i)
		process(list->s[i]);
}

static void
readflist(struct items *list, char *fname)
{
	FILE *fp;
	char line[FILENAME_MAX];
	unsigned char *s, *t;

	if ((fp = fopen(fname, "rb")) == NULL)
		die("ld: %s: %s", fname, strerror(errno));

	while (fgets(line, sizeof(line), fp)) {
		size_t n = strlen(line);
		if (n == 0)
			continue;
		if (line[n-1] != '\n')
			die("ld: %s: line too long", fname);
		for (s = line; isspace(*s); ++s)
			*s = '\0';
		for (t = &line[n-1]; isspace(*t); --t)
			*t = '\0';
		newitem(list, xstrdup(s));
	}

	if (ferror(fp))
		die("ld: %s: %s", fname, strerror(errno));

	fclose(fp);
}

static void
usage(void)
{
	fputs("usage: ld [options] [@file] file ...\n", stderr);
	exit(1);
}

int
main(int argc, char *argv[])
{
	unsigned i;
	struct items flist = {.n = 0};

	ARGBEGIN {
	case 's':
	case 'u':
	case 'l':
	case 'x':
	case 'X':
	case 'r':
	case 'd':
	case 'n':
	case 'i':
	case 'o':
	case 'e':
	case 'O':
	case 'D':
		break;
	default:
		usage();
	} ARGEND


	if (argc == 0)
		usage();

	if (*argv[0] == '@') {
		readflist(&flist, *argv + 1);
		++argv;
	}

	for (; *argv; ++argv)
		newitem(&flist, *argv);

	pass1(&flist);
	pass2(&flist);

	return 0;
}
