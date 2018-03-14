static char sccsid[] = "@(#) ./ld/coff32.c";

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../inc/coff32/filehdr.h"
#include "../inc/coff32/scnhdr.h"
#include "../inc/coff32/syms.h"
#include "../inc/scc.h"
#include "ld.h"

static int
pass1(char *fname, char *member, FILE *fp)
{
}

static int
pass2(char *fname, char *member, FILE *fp)
{
}

static int
probe(char *fname, char *member, FILE *fp)
{
	int c;
	int c1, c2;
	fpos_t pos;
	unsigned short magic;

	fgetpos(fp, &pos);
	c1 = getc(fp);
	c2 = getc(fp);
	fsetpos(fp, &pos);

	if (ferror(fp))
		die("nm: %s: %s", fname, strerror(errno));

	if (c1 == EOF || c2 == EOF)
		return 0;
	magic = c1 | c2 << 8;

	switch (magic) {
	case COFF_Z80MAGIC:
		return 1;
	default:
		return 0;
	}
}

struct objfile coff32 = {
	.probe = probe,
	.pass1 = pass1,
	.pass2 = pass2,
};
