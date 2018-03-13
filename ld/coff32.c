static char sccsid[] = "@(#) ./ld/coff32.c";

#include <stdio.h>

#include "ld.h"

static int
probe(char *fname, char *member, FILE *fp)
{
}

static int
pass1(char *fname, char *member, FILE *fp)
{
}

static int
pass2(char *fname, char *member, FILE *fp)
{
}

struct objfile coff32 = {
	.probe = probe,
	.pass1 = pass1,
	.pass2 = pass2,
};
