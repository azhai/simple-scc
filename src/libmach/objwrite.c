#include <stdio.h>

#include <scc/mach.h>

#include "libmach.h"

static int (*funv[])(Obj *, FILE *) = {
	[COFF32] = coff32write,
};

int
objwrite(Obj *obj, FILE *fp)
{
	int fmt;

	fmt = FORMAT(obj->type);
	if (fmt >= NFORMATS)
		return -1;

	if ((*funv[fmt])(obj, fp) < 0)
		return -1;
	return 0;
}
