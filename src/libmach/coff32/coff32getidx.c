#include <stdio.h>

#include <scc/mach.h>

#include "../libmach.h"
#include "coff32.h"

int
coff32getidx(long *nsyms, Objsymdef **def, FILE *fp)
{
	return coff32xgetidx(BIG_ENDIAN, nsyms, def, fp);
}
