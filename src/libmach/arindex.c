#include <stdio.h>

#include <scc/mach.h>

#include "libmach.h"

extern indexfun_t indexv[];

long
arindex(int type, long nsyms, Objsymdef *head, FILE *fp)
{
	int fmt;
	indexfun_t fn;

	fmt = FORMAT(type);
	if (fmt >= NFORMATS)
		return -1;
	fn = indexv[fmt];
	return (*fn)(type, nsyms, head, fp);
}
