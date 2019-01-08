#include <assert.h>
#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <scc/coff32/filehdr.h>
#include <scc/coff32/scnhdr.h>
#include <scc/coff32/syms.h>
#include <scc/mach.h>

#include "libmach.h"

typedef struct coff32 Coff32;

struct arch {
	char *name;
	unsigned char magic[2];
	int type;
};

struct coff32 {
	FILHDR hdr;
	SCNHDR *scns;
	SYMENT *ents;
	char *strtbl;
	unsigned long strsiz;
};

static struct arch archs[] = {
	"coff32-i386", "\x4c\x01", OBJ(COFF32, ARCH386, LITTLE_ENDIAN),
	"coff32-z80", "\x5a\x80", OBJ(COFF32, ARCHZ80, LITTLE_ENDIAN),
	NULL,
};

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

static int
probe(unsigned char *buf, char **name)
{
	struct arch *ap;

	for (ap = archs; ap->name; ap++) {
		if (ap->magic[0] == buf[0] && ap->magic[1] == buf[1]) {
			if (name)
				*name = ap->name;
			return ap->type;
		}
	}
	return -1;
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

static int
mkindex(Obj *obj)
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
	if (fseek(fp, hdr->f_opthdr, SEEK_CUR) < 0)
		return 0;
	for (i = 0; i < hdr->f_nscns; i++) {
		if (fread(buf, SCNHSZ, 1, fp) < 0)
			return 0;
		unpack_scn(ORDER(obj->type), buf, &scn[i]);
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

	if (fsetpos(fp, &obj->pos))
		return 0;
	if (fseek(fp, hdr->f_symptr, SEEK_CUR) < 0)
		return 0;
	for (i = 0; i < hdr->f_nsyms; i++) {
		if (fread(buf, SYMESZ, 1, fp) != 1)
			return 0;
		unpack_ent(ORDER(obj->type), buf, &ent[i]);
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
read(Obj *obj, FILE *fp)
{
	if (fgetpos(fp, &obj->pos))
		goto error;
	if (!readhdr(obj, fp))
		goto error;
	if (!readscns(obj, fp))
		goto error;
	if (!readents(obj, fp))
		goto error;
	if (!readstr(obj, fp))
		goto error;
	if (!mkindex(obj))
		goto error;
	return 0;

error:
	objreset(obj);
	return -1;
}

static int
write(Obj *obj, FILE *fp)
{
	return -1;
}

static void
del(Obj *obj)
{
	struct coff32 *coff = obj->data;

	if (coff) {
		free(coff->scns);
		free(coff->ents);
		free(coff->strtbl);
	}
	free(obj->data);
	obj->data = NULL;
}

static int
new(Obj *obj)
{
	struct coff32 *coff;

	if ((coff = calloc(1, sizeof(*coff))) == NULL)
		return -1;
	obj->data = coff;
	return 0;
}

static int
strip(Obj *obj)
{
	struct coff32 *coff = obj->data;
	FILHDR *hdr;

	if (coff) {
		hdr = &coff->hdr;
		free(coff->ents);
		coff->ents = NULL;
		hdr->f_nsyms = 0;
		hdr->f_symptr = 0;
	}
}

struct format objcoff32 = {
	.probe = probe,
	.new = new,
	.del = del,
	.read = read,
	.write = write,
	.strip = strip,
};
