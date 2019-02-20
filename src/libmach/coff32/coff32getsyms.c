#include <stdio.h>
#include <ctype.h>

#include <scc/mach.h>

#include "../libmach.h"
#include "coff32.h"

static int
typeof(Coff32 *coff, SYMENT *ent)
{
	int c;
	SCNHDR *scn;
	long flags;

	switch (ent->n_scnum) {
	case N_DEBUG:
		c = 'N';
		break;
	case N_ABS:
		c = 'a';
		break;
	case N_UNDEF:
		c = (ent->n_value != 0) ? 'C' : 'U';
		break;
	default:
		if (ent->n_scnum > coff->hdr.f_nscns)
			return -1;
		scn = &coff->scns[ent->n_scnum-1];
		flags = scn->s_flags;
		if (flags & STYP_TEXT)
			c = 't';
		else if (flags & STYP_DATA)
			c = 'd';
		else if (flags & STYP_BSS)
			c = 'b';
		else
			c = '?';
		break;
	}

	if (ent->n_sclass == C_EXT)
		c = toupper(c);

	return c;
}

static char *
symname(Coff32 *coff, SYMENT *ent)
{
	long off;

	if (ent->n_zeroes != 0)
		return ent->n_name;

	off = ent->n_offset;
	if (off >= coff->strsiz)
		return NULL;
	return &coff->strtbl[off];
}

int
coff32getsyms(Obj *obj)
{
	int t;
	long i;
	char *s;
	Objsym *sym;
	SYMENT *ent;
	Coff32 *coff = obj->data;

	for (i = 0; i < coff->hdr.f_nsyms; i += ent->n_numaux + 1) {
		ent = &coff->ents[i];

		if ((t = typeof(coff, ent)) < 0)
			return -1;

		if ((s = symname(coff, ent)) == NULL)
			return -1;

		if ((sym = objlookup(obj, s, 1)) == NULL)
			return -1;

		sym->type = t;
		sym->value = ent->n_value;
		sym->size = (sym->type == 'C') ? ent->n_value : 0;
	}

	return i;
}
