#include <stdio.h>

#include <scc/mach.h>

#include "libmach.h"

extern getsymsfun_t getsymsv[];

int
objsyms(Obj *obj)
{
	int fmt;
	getsymsfun_t fn;

	fmt = FORMAT(obj->type);
	if (fmt >= NFORMATS)
		return -1;

	fn = getsymsv[fmt];
	return  (*fn)(obj);
}
