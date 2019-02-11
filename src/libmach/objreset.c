#include <stdio.h>

#include <scc/mach.h>

#include "libmach.h"

extern delfun_t delv[];

int
objreset(Obj *obj)
{
	int fmt;
	delfun_t fn;

	fmt = FORMAT(obj->type);
	if (fmt < NFORMATS)
		return -1;
	fn = delv[fmt];
	(*fn)(obj);
	objfree(obj, FREESYM);
	return 0;
}
