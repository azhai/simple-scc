#include <stdio.h>
#include <stdlib.h>

#include <scc/mach.h>

#include "../libmach.h"
#include "coff32.h"

void
coff32strip(Obj *obj)
{
	struct coff32 *coff = obj->data;
	FILHDR *hdr;

	hdr = &coff->hdr;
	free(coff->ents);
	coff->ents = NULL;
	hdr->f_nsyms = 0;
	hdr->f_symptr = 0;
}
