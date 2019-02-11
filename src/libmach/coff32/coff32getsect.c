#include <stdio.h>
#include <stdlib.h>

#include <scc/mach.h>

#include "../libmach.h"
#include "coff32.h"

int
coff32getsect(Obj *obj, Objsect **sectp)
{
	int nsecs;
	unsigned sflags, type;
	unsigned long flags;
	FILHDR *hdr;
	struct coff32 *coff;
	SCNHDR *scn;
	Objsect *secs, *sp;

	coff  = obj->data;
	hdr = &coff->hdr;
	scn = coff->scns;

	nsecs = 0;
	secs = malloc(sizeof(Objsect) * hdr->f_nscns);
	if (!secs)
		return -1;

	for (sp = secs; sp < &secs[hdr->f_nscns]; sp++) {
		flags = scn->s_flags;

		if (flags & STYP_TEXT) {
			type = 'T';
			sflags = SALLOC | SRELOC | SLOAD | SEXEC | SREAD;
			if (flags & STYP_NOLOAD)
				sflags |= SSHARED;
		} else if (flags & STYP_DATA) {
			type = 'D';
			sflags = SALLOC | SRELOC | SLOAD | SWRITE | SREAD;
			if (flags & STYP_NOLOAD)
				sflags |= SSHARED;
		} else if (flags & STYP_BSS) {
			type = 'B';
			sflags = SALLOC | SREAD | SWRITE;
		} else if (flags & STYP_INFO) {
			type = 'N';
			sflags = 0;
		} else if (flags & STYP_LIB) {
			type = 'T';
			sflags = SRELOC;
		} else if (flags & STYP_DSECT) {
			type = 'D';
			sflags = SRELOC;
		} else if (flags & STYP_PAD) {
			type = 'D';
			sflags = SLOAD;
		} else {
			type = 'D';  /* We assume that STYP_REG is data */
			sflags = SALLOC | SRELOC | SLOAD | SWRITE | SREAD;
		}

		if (flags & STYP_NOLOAD)
			sflags &= ~SLOAD;

		sp->name = scn->s_name;
		sp->offset = scn->s_scnptr;
		sp->size = scn->s_size;
		sp->type = type;
		nsecs++;
	}

	return 1;
}
