#include <stdio.h>

#include <scc/mach.h>

#include "../libmach.h"
#include "coff32.h"

Map *
coff32loadmap(Obj *obj, FILE *fp)
{
	long i;
	Map *map;
	long nsec;
	FILE *src;
	SCNHDR *scn;
	struct coff32 *coff = obj->data;
	FILHDR *hdr = &coff->hdr;

	nsec = hdr->f_nscns;
	if ((map = newmap(NULL, nsec)) == NULL)
		return NULL;

	for (scn = coff->scns; nsec--; ++scn) {
		unsigned long o;
		unsigned long long b = scn->s_paddr;
		unsigned long long e = b + scn->s_size;

		if (scn->s_scnptr != 0) {
			o = obj->pos + scn->s_scnptr;
			src = fp;
		} else {
			o = 0;
			src = NULL;
		}

		setmap(map, scn->s_name, src, b, e, o);
	}

	return map;
}
