#include <stdio.h>

#include <scc/mach.h>

#include "libmach.h"

static long (*funv[])(int, long, Objsymdef *, FILE *) = {
	[COFF32] = coff32setidx,
};

long
setindex(int type, long nsyms, Objsymdef *head, FILE *fp)
{
	int fmt;

	fmt = FORMAT(type);
	if (fmt >= NFORMATS)
		return -1;

	return (*funv[fmt])(type, nsyms, head, fp);
}
