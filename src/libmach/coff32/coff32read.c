#include <assert.h>
#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <scc/mach.h>

#include "../libmach.h"
#include "coff32.h"

static void
unpack_hdr(int order, unsigned char *buf, FILHDR *hdr)
{
	int n;

	n = unpack(order,
	           buf,
	           "sslllss",
	           &hdr->f_magic,
	           &hdr->f_nscns,
	           &hdr->f_timdat,
	           &hdr->f_symptr,
	           &hdr->f_nsyms,
	           &hdr->f_opthdr,
	           &hdr->f_flags);
	assert(n == FILHSZ);
}

static void
unpack_line(int order, unsigned char *buf, LINENO *lp)
{
	int n;

	n = unpack(order,
	           buf,
	           "lls",
	           &lp->l_symndx,
	           &lp->l_paddr,
	           &lp->l_lnno);
	assert(n == LINESZ);
}

static void
unpack_scn(int order, unsigned char *buf, SCNHDR *scn)
{
	int n;

	n = unpack(order,
	          buf,
	          "'8llllllssl",
	          scn->s_name,
	          &scn->s_paddr,
	          &scn->s_vaddr,
	          &scn->s_size,
	          &scn->s_scnptr,
	          &scn->s_relptr,
	          &scn->s_lnnoptr,
	          &scn->s_nrelloc,
	          &scn->s_nlnno,
	          &scn->s_flags);
	assert(n == SCNHSZ);
}

static void
unpack_ent(int order, unsigned char *buf, SYMENT *ent)
{
	int n;
	char *s;

	n = unpack(order,
	           buf,
	           "'8lsscc",
	           ent->n_name,
	           &ent->n_value,
	           &ent->n_scnum,
	           &ent->n_type,
	           &ent->n_sclass,
	           &ent->n_numaux);
	assert(n == SYMESZ);

	s = ent->n_name;
	if (!s[0] && !s[1] && !s[2] && !s[3])
		unpack(order, "ll", buf, &ent->n_zeroes, &ent->n_offset);
}

static void
unpack_reloc(int order, unsigned char *buf, RELOC *rel)
{
	int n;

	n = unpack(order,
	           buf,
	           "lls",
	           &rel->r_vaddr,
	           &rel->r_symndx,
	           &rel->r_type);
	assert(n == RELSZ);
}

static void
unpack_aout(int order, unsigned char *buf, AOUTHDR *aout)
{
	int n;

	n = unpack(order,
	           buf,
	           "ssllllll",
	           &aout->magic,
	           &aout->vstamp,
	           &aout->tsize,
	           &aout->dsize,
	           &aout->bsize,
	           &aout->entry,
	           &aout->text_start,
	           &aout->data_start);
	assert(n == AOUTSZ);
}

static int
readhdr(Obj *obj, FILE *fp)
{
	FILHDR *hdr;
	struct coff32 *coff;
	unsigned char buf[FILHSZ];

	coff  = obj->data;
	hdr = &coff->hdr;

	if (fread(buf, FILHSZ, 1, fp) != 1)
		return 0;
	unpack_hdr(ORDER(obj->type), buf, hdr);

	return 1;
}

static int
loadsections(Obj *obj, FILE *fp)
{
	size_t len;
	unsigned sflags, type;
	unsigned long flags;
	FILHDR *hdr;
	struct coff32 *coff;
	SCNHDR *scn;
	Section *secs, *sp;

	coff  = obj->data;
	hdr = &coff->hdr;
	scn = coff->scns;

	secs = malloc(sizeof(Section) * hdr->f_nscns);
	if (!secs)
		return 0;
	obj->sections = secs;

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

		len = strlen(scn->s_name) + 1;
		if ((sp->name = malloc(len)) == NULL)
			return 0;

		memcpy(sp->name, scn->s_name, len);
		sp->fp = fp;
		sp->offset = scn->s_scnptr;
		sp->size = scn->s_size;
		sp->type = type;
		obj->nsecs++;
	}
	return 1;
}

static int
readstr(Obj *obj, FILE *fp)
{
	FILHDR *hdr;
	struct coff32 *coff;
	long siz;
	char *str;
	unsigned char buf[10];

	coff  = obj->data;
	hdr = &coff->hdr;

	if (hdr->f_nsyms == 0)
		return 1;

	if (fread(buf, 4, 1, fp) != 1)
		return 0;
	unpack(ORDER(obj->type), buf, "l", &siz);
	siz -= 4;
	if (siz < 0)
		return 0;
	if (siz > 0) {
		if (siz > SIZE_MAX)
			return 0;
		str = malloc(siz);
		if (!str)
			return 0;
		coff->strtbl = str;
		coff->strsiz = siz;

		if (fread(str, siz, 1, fp) != 1)
			return 0;
	}
	return 1;
}

static int
readreloc(Obj *obj, FILE *fp)
{
	int i;
	long j;
	RELOC **rels, *rp;
	SCNHDR *scn;
	FILHDR *hdr;
	struct coff32 *coff;
	unsigned char buf[RELSZ];

	coff  = obj->data;
	hdr = &coff->hdr;

	rels = calloc(obj->nsecs, sizeof(*rels));
	if (!rels)
		return 0;
	coff->rels = rels;

	for (i = 0; i < hdr->f_nscns; i++) {
		scn = &coff->scns[i];
		if (scn->s_nrelloc == 0)
			continue;

		if (!objpos(obj, fp, scn->s_relptr))
			return 0;

		rp = calloc(scn->s_nrelloc, sizeof(RELOC));
		if (!rp)
			return 0;
		rels[i] = rp;

		for (j = 0; j < scn->s_nrelloc; j++) {
			if (fread(buf, RELSZ, 1, fp) != 1)
				return 0;
			unpack_reloc(ORDER(obj->type), buf, &rp[i]);
		}
	}

	return 1;
}

static int
readents(Obj *obj, FILE *fp)
{
	FILHDR *hdr;
	struct coff32 *coff;
	SYMENT *ent;
	long i;
	unsigned char buf[SYMESZ];

	coff  = obj->data;
	hdr = &coff->hdr;

	if (hdr->f_nsyms == 0)
		return 1;

	ent = calloc(hdr->f_nsyms, sizeof(*ent));
	if (!ent)
		return 0;
	coff->ents = ent;

	if (!objpos(obj, fp, hdr->f_symptr))
		return 0;
	for (i = 0; i < hdr->f_nsyms; i++) {
		if (fread(buf, SYMESZ, 1, fp) != 1)
			return 0;
		unpack_ent(ORDER(obj->type), buf, &ent[i]);
	}

	return 1;
}

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

static int
loadsyms(Obj *obj)
{
	int t;
	long i;
	char *s;
	Symbol *sym;
	SYMENT *ent;
	Coff32 *coff = obj->data;

	for (i = 0; i < coff->hdr.f_nsyms; i += ent->n_numaux + 1) {
		ent = &coff->ents[i];

		if ((t = typeof(coff, ent)) < 0)
			return 0;

		if ((s = symname(coff, ent)) == NULL)
			return 0;

		if ((sym = objlookup(obj, s)) == NULL)
			return 0;

		sym->type = t;
		sym->value = ent->n_value;
		sym->size = (sym->type == 'C') ? ent->n_value : 0;
	}

	return 1;
}

static int
readscns(Obj *obj, FILE *fp)
{
	FILHDR *hdr;
	struct coff32 *coff;
	SCNHDR *scn;
	long i;
	unsigned char buf[SCNHSZ];

	coff  = obj->data;
	hdr = &coff->hdr;

	if (hdr->f_nscns > 0) {
		scn = calloc(hdr->f_nscns, sizeof(*scn));
		if (!scn)
			return 0;
		coff->scns = scn;
	}

	for (i = 0; i < hdr->f_nscns; i++) {
		if (fread(buf, SCNHSZ, 1, fp) < 0)
			return 0;
		unpack_scn(ORDER(obj->type), buf, &scn[i]);
	}

	return 1;
}

static int
readlines(Obj *obj, FILE *fp)
{
	int i,j;
	LINENO **lines, *lp;
	FILHDR *hdr;
	SCNHDR *scn;
	struct coff32 *coff;
	unsigned char buf[LINESZ];

	coff  = obj->data;
	hdr = &coff->hdr;

	lines = calloc(sizeof(lp), hdr->f_nscns);
	if (!lines)
		return 0;
	coff->lines = lines;

	for (i = 0; i < hdr->f_nscns; i++) {
		scn = &coff->scns[i];
		if (scn->s_nlnno == 0)
			continue;

		lp = calloc(sizeof(*lp), scn->s_nlnno);
		if (!lp)
			return 0;
		lines[i] = lp;

		for (j = 0; j < scn->s_nlnno; j++) {
			if (!objpos(obj, fp, scn->s_lnnoptr))
				return 0;
			if (fread(buf, LINESZ, 1, fp) == 1)
				return 0;
			unpack_line(ORDER(obj->type), buf, &lp[j]);
		}
	}

	return 1;
}

static int
readaout(Obj *obj, FILE *fp)
{
	FILHDR *hdr;
	struct coff32 *coff;
	unsigned char buf[AOUTSZ];

	coff  = obj->data;
	hdr = &coff->hdr;

	if (hdr->f_opthdr == 0)
		return 1;

	if (fread(buf, AOUTSZ, 1, fp) != 1)
		return 0;

	coff->aout = malloc(sizeof(AOUTHDR));
	if (!coff->aout)
		return 0;

	unpack_aout(ORDER(obj->type), buf, coff->aout);

	return 1;
}

int
coff32read(Obj *obj, FILE *fp)
{
	/* TODO: Add validation of the different fields */
	if (fgetpos(fp, &obj->pos))
		goto error;
	if (!readhdr(obj, fp))
		goto error;
	if (!readaout(obj, fp))
		goto error;
	if (!readscns(obj, fp))
		goto error;
	if (!readents(obj, fp))
		goto error;
	if (!readstr(obj, fp))
		goto error;
	if (!readreloc(obj, fp))
		goto error;
	if (!readlines(obj, fp))
		goto error;
	if (!loadsyms(obj))
		goto error;
	if (!loadsections(obj, fp))
		goto error;
	return 0;

error:
	objreset(obj);
	return -1;
}
