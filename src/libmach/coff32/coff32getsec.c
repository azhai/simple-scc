#include <stdio.h>

#include <scc/mach.h>

#include "../libmach.h"
#include "coff32.h"

Section *
coff32getsec(Obj *obj, int *idx, Section *sec)
{
	long n = *idx;
	int type;
	unsigned sflags;
	SCNHDR *scn;
	Coff32 *coff = obj->data;
	FILHDR *hdr = &coff->hdr;

	if (n >= hdr->f_nscns)
		return NULL;

	scn = &coff->scns[n];
	switch (scn->s_flags) {
	case STYP_REG:
		type = 'D';
		sflags = SALLOC | SRELOC | SLOAD | SWRITE | SREAD;
		break;
	case STYP_DSECT:
		type = '?';
		sflags = SRELOC;
		break;
	case STYP_NOLOAD:
		type = 'D';
		sflags = SALLOC | SREAD | SWRITE;
		break;
	case TYP_REVERSE_PAD:
	case STYP_PAD:
		type = '?';
		sflags = SLOAD;
		break;
	case STYP_COPY:
		type = '?';
		sflags |= SLOAD | SRELOC;
		break;
	case STYP_TEXT:
		type = 'T';
		sflags = SALLOC | SRELOC | SLOAD | SEXEC | SREAD;
		break;
	case STYP_DATA:
		type = 'D';
		sflags = SALLOC | SRELOC | SLOAD | SWRITE | SREAD;
		break;
	case STYP_BSS:
		type = 'B';
		sflags = SALLOC | SREAD | SWRITE;
		break;
	case STYP_LIT:
	case STYP_RDATA:
		type = 'D';
		sflags = SALLOC | SRELOC | SLOAD | SWRITE | SREAD;
		break;
	case STYP_LIB:
	case STYP_INFO:
		type = 'N';
		sflags = 0;
		break;
	case STYP_OVER:
		type = '?';
		sflags = SRELOC;
		break;
	case STYP_GROUP:
	case STYP_MERGE:
	default:
		type = '?';
		sflags = 0;
		break;
	}

	sec->name = scn->s_name;
	sec->index = n;
	sec->size = scn->s_size;
	sec->base = scn->s_vaddr;
	sec->type = type;
	sec->flags = sflags;
	sec->align = 16;

	return sec;
}
