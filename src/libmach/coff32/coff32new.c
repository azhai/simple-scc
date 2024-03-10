#include <stdio.h>
#include <stdlib.h>

#include <scc/mach.h>
#include <scc/scc.h>

#include "../libmach.h"
#include "coff32.h"

int
coff32new(Obj *obj, int type)
{
	struct arch *p;
	struct coff32 *coff;
	FILHDR *hdr;

	for (p = coff32archs; p->name && p->type != type; ++p)
		;
	if (!p->name)
		return -1;

	if ((coff = calloc(1, sizeof(*coff))) == NULL)
		return -1;

	hdr = &coff->hdr;
	hdr->f_magic = p->magic[1] << 8 | p->magic[0];
	hdr->f_flags = p->flags;

	obj->data = coff;
	obj->index = "/";
	return 0;
}
