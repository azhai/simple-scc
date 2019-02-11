#include <stdio.h>

#include <scc/mach.h>

#include "libmach.h"

extern stripfun_t stripv[];

int
objstrip(Obj *obj)
{
	int fmt;
	stripfun_t fn;

	fmt = FORMAT(obj->type);
	if (fmt >= NFORMATS)
		return -1;
	fn = stripv[fmt];
	(*fn)(obj);

	objfree(obj, GENERICDEL);

	return 0;
}
