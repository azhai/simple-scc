#include <stdio.h>

#include <scc/mach.h>

#include "libmach.h"

static int (*funv[])(Obj *) = {
	[COFF32] = coff32getsect,
};

int
objsect(Obj *obj)
{
	int fmt;

	fmt = FORMAT(obj->type);
	if (fmt >= NFORMATS)
		return -1;

	return  (*funv[fmt])(obj);
}
