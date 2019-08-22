#include <stdio.h>

#include <scc/mach.h>

#include "libmach.h"

static int (*freadv[])(Obj *, FILE *) = {
	[COFF32] = coff32read,
};

int
objread(Obj *obj, FILE *fp)
{
	int fmt;

	fmt = FORMAT(obj->type);
	if (fmt >= NFORMATS)
		return -1;

	if ((*freadv[fmt])(obj, fp) < 0)
		return -1;

	return 0;
}
