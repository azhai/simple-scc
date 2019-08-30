#include <stdio.h>

#include <scc/mach.h>

#include "../libmach.h"
#include "coff32.h"

Map *
coff32loadmap(Obj *obj, FILE *fp)
{
	long i;
	Map *map;
	SCNHDR *scn;
	struct coff32 *coff = obj->data;
	FILHDR *hdr = &coff->hdr;
	long nsec = hdr->f_nscns;

	if ((map = newmap(nsec, fp)) == NULL)
		return NULL;

	for (scn = coff->scns; nsec--; ++scn)
		setmap(map, scn->s_name, obj->pos + scn->s_scnptr);

	return map;
}
