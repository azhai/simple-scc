#include <stdio.h>

#include <scc/mach.h>

#include "libmach.h"

extern getsectfun_t getsectv[];

int
objsect(Obj *obj, Objsect **secp)
{
	int fmt;
	getsectfun_t fn;

	fmt = FORMAT(obj->type);
	if (fmt >= NFORMATS)
		return -1;

	fn = getsectv[fmt];
	return  (*fn)(obj, secp);
}
