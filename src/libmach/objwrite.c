#include <stdio.h>

#include <scc/mach.h>

#include "libmach.h"

extern writefun_t writev[];

int
objwrite(Obj *obj, FILE *fp)
{
	int fmt;
	writefun_t fn;

	fmt = FORMAT(obj->type);
	if (fmt >= NFORMATS)
		return -1;
	fn = writev[fmt];
	if ((*fn)(obj, fp) < 0)
		return -1;
	return 0;
}
