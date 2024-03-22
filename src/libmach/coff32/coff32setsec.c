#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <scc/mach.h>

#include "../libmach.h"
#include "coff32.h"

Section *
coff32setsec(Obj *obj, int *idx, Section *sec)
{
	long flags, n = *idx;
	SCNHDR *scn;
	Coff32 *coff = obj->data;
	FILHDR *hdr = &coff->hdr;

	switch (sec->type) {
	case 'D':
		switch (sec->flags) {
		case SALLOC | SRELOC | SLOAD | SWRITE | SREAD:
			if (strcmp(sec->name ,".data") == 0)
				flags = STYP_DATA;
			else
				flags = STYP_REG;
			break;
		case SALLOC | SREAD | SWRITE:
			flags = STYP_NOLOAD;
			break;
		case SALLOC | SRELOC | SLOAD | SREAD:
			flags = STYP_RDATA;
			break;
		case SEXEC | SALLOC | SRELOC | SLOAD | SREAD:
			flags = STYP_TEXT | STYP_DATA;
			break;
		default:
			goto invalid;
		}
		break;
	case 'T':
		flags = STYP_TEXT;
		break;
	case 'B':
		flags = STYP_BSS;
		break;
	case 'N':
	case '?':
	default:
	invalid:
		/* TODO */
		return NULL;
	}

	if (strlen(sec->name) >= SCNNMLEN)
		return NULL;

	if (n >= hdr->f_nscns) {
		if (n > SHRT_MAX - 1)
			return NULL;
		scn = realloc(coff->scns, (n+1) * sizeof(SCNHDR));
		if (!scn)
			return NULL;
		coff->scns = scn;
		hdr->f_nscns = n + 1;
	}
	scn = &coff->scns[n];

	strncpy(scn->s_name, sec->name, SCNNMLEN);
	scn->s_paddr = 0;
	scn->s_vaddr = sec->base;
	scn->s_size = sec->size;
	scn->s_scnptr = 0;
	scn->s_relptr = 0;
	scn->s_lnnoptr = 0;
	scn->s_nrelloc = 0;
	scn->s_nlnno = 0;
	scn->s_flags = flags; 

	return sec;
}
