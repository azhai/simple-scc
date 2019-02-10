#include <stdio.h>

#include <scc/mach.h>

#include "../libmach.h"
#include "coff32.h"

long
coff32setindex(int type, long nsymbols, Objsymdef *head, FILE *fp)
{
	return coff32setidx(BIG_ENDIAN, nsymbols, head, fp);
}
