#include <stdio.h>

#include <scc/mach.h>

#include "libmach.h"

extern getidxfun_t getidxv[];

int
getindex(int type, long *nsyms, Objsymdef **head, FILE *fp)
{
	int fmt;
	getidxfun_t fn;

	fmt = FORMAT(type);
	if (fmt >= NFORMATS)
		return -1;
	fn = getidxv[fmt];
	return (*fn)(type, nsyms, head, fp);
}
