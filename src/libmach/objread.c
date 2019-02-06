#include <stdio.h>

#include <scc/mach.h>

#include "libmach.h"

extern readfun_t readv[];

int
objread(Obj *obj, FILE *fp)
{
	int fmt;
	readfun_t fn;

	fmt = FORMAT(obj->type);
	if (fmt >= NFORMATS)
		return -1;

	fn = readv[fmt];
	if ((*fn)(obj, fp) < 0)
		return -1;
	return 0;
}
