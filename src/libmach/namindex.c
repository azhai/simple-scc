#include <stdio.h>

#include <scc/mach.h>

#include "libmach.h"

extern namidxfun_t namidxv[];

char *
namindex(int type)
{
	int fmt;
	namidxfun_t fn;

	fmt = FORMAT(type);
	if (fmt >= NFORMATS)
		return NULL;
	fn = namidxv[fmt];
	return (*fn)();
}
