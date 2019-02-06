#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <scc/mach.h>

#include "../libmach.h"
#include "coff32.h"

static void
pack_hdr(int order, unsigned char *buf, FILHDR *hdr)
{
	int n;

	n = pack(order,
	         buf,
	         "sslllss",
	         hdr->f_magic,
	         hdr->f_nscns,
	         hdr->f_timdat,
	         hdr->f_symptr,
	         hdr->f_nsyms,
	         hdr->f_opthdr,
	         hdr->f_flags);
	assert(n == FILHSZ);
}

static void
pack_scn(int order, unsigned char *buf, SCNHDR *scn)
{
	int n;

	n = pack(order,
	         buf,
	         "'8llllllssl",
	         scn->s_name,
	         scn->s_paddr,
	         scn->s_vaddr,
	         scn->s_size,
	         scn->s_scnptr,
	         scn->s_relptr,
	         scn->s_lnnoptr,
	         scn->s_nrelloc,
	         scn->s_nlnno,
	         scn->s_flags);
	assert(n == SCNHSZ);
}

static void
pack_ent(int order, unsigned char *buf, SYMENT *ent)
{
	int n;
	char *s;

	/* TODO: What happens with the union? */

	n = pack(order,
	         buf,
	         "'8lsscc",
	         ent->n_name,
	         &ent->n_value,
	         &ent->n_scnum,
	         &ent->n_type,
	         &ent->n_sclass,
	         &ent->n_numaux);
	assert(n == SYMESZ);
}

static void
pack_aout(int order, unsigned char *buf, AOUTHDR *aout)
{
	int n;

	n = unpack(order,
	           buf,
	           "ssllllll",
	           aout->magic,
	           aout->vstamp,
	           aout->tsize,
	           aout->dsize,
	           aout->bsize,
	           aout->entry,
	           aout->text_start,
	           aout->data_start);
	assert(n == AOUTSZ);
}

static void
pack_reloc(int order, unsigned char *buf, RELOC *rel)
{
	int n;

	n = pack(order,
	         buf,
	         "lls",
	         rel->r_vaddr,
	         rel->r_symndx,
	         rel->r_type);
	assert(n == RELSZ);
}

static void
pack_line(int order, unsigned char *buf, LINENO *lp)
{
	int n;

	n = pack(order,
	         buf,
	         "lls",
	         lp->l_symndx,
	         lp->l_paddr,
	         lp->l_lnno);
	assert(n == LINESZ);
}

static int
writehdr(Obj *obj, FILE *fp)
{
	FILHDR *hdr;
	struct coff32 *coff;
	unsigned char buf[FILHSZ];

	coff  = obj->data;
	hdr = &coff->hdr;

	pack_hdr(ORDER(obj->type), buf, hdr);
	if (fwrite(buf, FILHSZ, 1, fp) != 1)
		return 0;

	return 1;
}

static int
writescns(Obj *obj, FILE *fp)
{
	/* TODO */
}

static int
writeents(Obj *obj, FILE *fp)
{
	/* TODO */
}

static int
writestr(Obj *obj, FILE *fp)
{
	/* TODO */
}

int
coff32write(Obj *obj, FILE *fp)
{
	struct coff32 *coff;

	coff  = obj->data;
	coff->strsiz = 0;
	free(coff->strtbl);

	if (!writehdr(obj, fp))
		return -1;
	if (!writescns(obj, fp))
		return -1;
	if (!writeents(obj, fp))
		return -1;
	if (!writestr(obj, fp))
		return -1;

	return 0;
}
