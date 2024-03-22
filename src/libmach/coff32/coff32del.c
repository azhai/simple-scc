#include <stdio.h>
#include <stdlib.h>

#include <scc/mach.h>

#include "../libmach.h"
#include "coff32.h"

void
coff32del(Obj *obj)
{
	int i;
	struct coff32 *coff = obj->data;

	if (coff) {
		free(coff->scns);
		free(coff->ents);
		free(coff->strtbl);

		for (i = 0; i < coff->hdr.f_nscns; i++) {
			free(coff->rels[i]);
			free(coff->lines[i]);
		}
		free(coff->rels);
		free(coff->lines);
	}
	free(obj->data);
	obj->data = NULL;
}
