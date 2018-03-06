
static char sccsid[] = "@(#) ./nm/coff.c";

#include <stdio.h>

#include "nm.h"

static void
nm(char *fname, FILE *fp)
{
}

static int
probe(FILE *fp)
{
}

struct objfile coff = {
	.probe = probe,
	.nm = nm,
};
