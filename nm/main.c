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
int radix = 16;
int Pflag;
int Aflag;
int vflag;
int gflag;
int uflag;
int arflag;

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
