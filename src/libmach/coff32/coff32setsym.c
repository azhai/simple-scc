#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <scc/mach.h>

#include "../libmach.h"
#include "coff32.h"

static int
defent(Coff32 *coff, SYMENT *ent, Symbol *sym)
{
	ent->n_scnum = sym->section + 1;

	switch (sym->type) {
	case 'N':
		/*
		 * TODO: What happens with .indent ?
		 *       look if the section is STYP_INFO 
		 */
		ent->n_scnum = N_DEBUG;
		break;
	case 'A':
		ent->n_sclass = C_EXT;
	case 'a':
		ent->n_scnum = N_ABS;
		break;
	case 'C':
	case 'U':
		ent->n_scnum = N_UNDEF;
		break;
	case 'T':
	case 'D':
	case 'B':
	case 'R':
		ent->n_sclass = C_EXT;
		break;
	case 't':
	case 'd':
	case 'b':
	case 'r':
		ent->n_sclass = C_STAT;
		break;
	case '?':
	default:
		/* TODO */
		return -1;
	}

	return 0;
}

static char *
symname(Coff32 *coff, SYMENT *ent, Symbol *sym)
{
	char *p;
	unsigned long siz = strlen(sym->name);

	if (siz < SYMNMLEN)
		return strncpy(ent->n_name, sym->name, SYMNMLEN);

	if (coff->strsiz > ULONG_MAX - siz - 1)
		return NULL;

	siz += coff->strsiz + 1;
	if ((p = realloc(coff->strtbl, siz)) == NULL)
		return NULL;
	coff->strtbl = p;

	ent->n_zeroes = 0;
	ent->n_offset = coff->strsiz;
	coff->strsiz += siz;
	return strcpy(&coff->strtbl[ent->n_offset], sym->name);
}

Symbol *
coff32setsym(Obj *obj, int *idx, Symbol *sym)
{
	int n = *idx;
	SYMENT *ent, *p;
	Coff32 *coff = obj->data;
	FILHDR *hdr = &coff->hdr;

	hdr->f_flags &= ~F_LSYMS;
	if (n >= coff->hdr.f_nsyms) {
		if (n > LONG_MAX-1)
			return NULL;
		if ((p = realloc(coff->ents, (n+1) * sizeof(*p))) == NULL)
			return NULL;
		coff->ents = p;
		coff->hdr.f_nsyms = n+1;
	}
	ent = &coff->ents[n];
	if (!symname(coff, ent, sym))
		return NULL;

	ent->n_value = sym->value;
	if (defent(coff, ent, sym) < 0)
		return NULL;

	/*
	 * TODO: 
	 *      sym->stype
	 */
	ent->n_numaux = 0; /* TODO: debug information */

	*idx += ent->n_numaux;

	return sym;
}
