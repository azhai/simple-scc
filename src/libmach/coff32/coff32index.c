#include <stdio.h>

#include <scc/mach.h>

#include "../libmach.h"
#include "coff32.h"

long
coff32index(int type, long nsymbols, Objsymdef *head, FILE *fp)
{
	return coff32idx(BIG_ENDIAN, nsymbols, head, fp);
}
