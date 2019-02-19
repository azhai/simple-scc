#include <stdio.h>

#include <scc/mach.h>

#include "libmach.h"

static void (*funv[])(Obj *) = {
	[COFF32] = coff32strip,
};

int
objstrip(Obj *obj)
{
	int fmt;

	fmt = FORMAT(obj->type);
	if (fmt >= NFORMATS)
		return -1;
	(*funv[fmt])(obj);

	objfree(obj, GENERICDEL);

	return 0;
}
