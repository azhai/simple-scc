#include <stdio.h>

#include <scc/mach.h>

#include "../libmach.h"
#include "coff32.h"

int
coff32getindex(int type, long *nsyms, Objsymdef **def, FILE *fp)
{
	return coff32getidx(BIG_ENDIAN, nsyms, def, fp);
}
