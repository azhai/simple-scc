#include <stdio.h>

#include <scc/mach.h>

#include "libmach.h"

extern setidxfun_t setidxv[];

long
setindex(int type, long nsyms, Objsymdef *head, FILE *fp)
{
	int fmt;
	setidxfun_t fn;

	fmt = FORMAT(type);
	if (fmt >= NFORMATS)
		return -1;
	fn = setidxv[fmt];
	return (*fn)(type, nsyms, head, fp);
}
