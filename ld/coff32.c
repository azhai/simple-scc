static char sccsid[] = "@(#) ./ld/coff32.c";

#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../inc/coff32/filehdr.h"
#include "../inc/coff32/scnhdr.h"
#include "../inc/coff32/syms.h"
#include "../inc/scc.h"
#include "ld.h"

static FILHDR *
getfhdr(Obj *obj, unsigned char *buff, FILHDR *hdr)
{
	int n;

	n = (*obj->unpack)(buff,
	                   "sslllss",
	                   &hdr->f_magic,
	                   &hdr->f_nscns,
	                   &hdr->f_timdat,
	                   &hdr->f_symptr,
	                   &hdr->f_nsyms,
	                   &hdr->f_opthdr,
	                   &hdr->f_flags);
	assert(n == FILHSZ);
	return hdr;
}

static int
readstr(Obj *obj, long off)
{
	unsigned char buff[4];
	char *str;
	size_t siz;

	if (fseek(obj->fp, off, SEEK_SET) == EOF)
		return -1;

	if (fread(buff, 4, 1, obj->fp) != 1)
		return -1;

	(*obj->unpack)(buff, "l", &siz);

	siz -= 4;
	if (siz == 0) {
		obj->strtbl = NULL;
		obj->strsiz = 0;
		return 0;
	}

	if (siz > SIZE_MAX || (str = malloc(siz)) == NULL)
		outmem();

	if (fread(str, siz, 1, obj->fp) != 1)
		return -1;

	obj->strtbl = str;
	obj->strsiz = siz;

	return 0;
}

static SCNHDR *
getscn(Obj *obj, unsigned char *buff, SCNHDR *scn)
{
	int n;

	n = (*obj->unpack)(buff,
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
	return scn;
}

static int
readsects(Obj *obj, long off)
{
	unsigned a, nsec, i;
	unsigned char buff[SCNHSZ];
	SCNHDR *scn, *p;
	FILHDR *hdr;
	Symbol *sym, **sec;

	hdr = obj->filhdr;
	nsec = hdr->f_nscns;

	if (nsec > SIZE_MAX / sizeof(*scn))
		return -1;

	if (nsec > SIZE_MAX / sizeof(Symbol *))
		return -1;

	scn = malloc(nsec * sizeof(*scn));
	sec = malloc(nsec * sizeof(Symbol *));
	if (!scn || !sec)
		outmem();
	obj->sections = sec;
	obj->scnhdr = scn;

	if (fseek(obj->fp, off, SEEK_SET) == EOF)
		return -1;

	a = obj->align - 1;
	for (i = 0; i < nsec; ++i) {
		p = &scn[i];
		if (fread(buff, SCNHSZ, 1, obj->fp) != 1)
			return -1;
		getscn(obj, buff, p);
		sym = lookup(p->s_name);

		sym->size = (sym->size + a) & a;
		if (sym->size > ULLONG_MAX - p->s_size) {
			fprintf(stderr,
			        "ld: %s: overflow in section '%s'\n",
			        obj->fname, p->s_name);
			exit(EXIT_FAILURE);
		}
		sym->size += p->s_size;
		obj->sections[i] = sym;
		
	}

	return 0;
}

static void
getsym(Obj *obj, unsigned char *buff, SYMENT *ent)
{
	int n;
	long off, zero;
	char *name;

	n = (*obj->unpack)(buff,
		           "'8lsscc",
		           &ent->n_name,
		           &ent->n_value,
		           &ent->n_scnum,
		           &ent->n_type,
		           &ent->n_sclass,
		           &ent->n_numaux);
	assert(n == SYMESZ);

	name = ent->n_name;
	if (!name[0] && !name[1] && !name[2] && !name[3])
		(*obj->unpack)(buff, "ll", &ent->n_zeroes, &ent->n_offset);
}

static char *
symname(Obj *obj, SYMENT *ent)
{
	long off;

	if (ent->n_zeroes != 0)
		return ent->n_name;

	off = ent->n_offset;
	if (off >= obj->strsiz) {
		fprintf(stderr,
		        "ld: invalid offset in symbol table: %zd\n", off);
		return "";
	}

	return &obj->strtbl[off];
}

static char
typeof(Obj *obj, SYMENT *ent)
{
	SCNHDR *sec;
	FILHDR *hdr;
	int c, n;
	long flags;

	switch (ent->n_scnum) {
	case N_DEBUG:
		c = 'n';
		break;
	case N_ABS:
		c = 'a';
		break;
	case N_UNDEF:
		c = (ent->n_value != 0) ? 'C' : 'U';
		break;
	default:
		sec = obj->scnhdr;
		hdr = obj->filhdr;
		n = ent->n_scnum;
		if (n > hdr->f_nscns)
			return '?';
		sec = &sec[n-1];
		flags = sec->s_flags;
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

static TUINT
getval(Obj *obj, SYMENT *ent)
{
	FILHDR *hdr = obj->filhdr;;
	SCNHDR *scn;
	Symbol *sym;
	unsigned n;

	if (ent->n_scnum > hdr->f_nscns) {
		fprintf(stderr,
		        "ld: %s: incorrect section number\n",
		        obj->fname,
		        ent->n_scnum);
		exit(EXIT_FAILURE);
	}

	n = ent->n_scnum-1;
	scn = obj->scnhdr;
	scn = &scn[n];
	sym = obj->sections[n];


	return ent->n_value + (sym->size - scn->s_size);
}

static int
readsyms(Obj *obj, long off)
{
	int type;
	unsigned i, n, aux;
	FILHDR *hdr = obj->filhdr;;

	if (fseek(obj->fp, off, SEEK_SET) == EOF)
		return -1;

	if (hdr->f_nsyms > SIZE_MAX / sizeof(Symbol *)) {
		fprintf(stderr,
		        "ld: %s: overflow in size of symbol redirection\n",
		        obj->fname);
		exit(EXIT_FAILURE);
	}
	obj->symbols = malloc(sizeof(Symbol *) * sizeof(Symbol *));
	if (!obj->symbols)
		outmem();

	hdr = obj->filhdr;
	aux = n = 0;
	for (i = 0; i < hdr->f_nsyms; i++) {
		Symbol *sym;
		TUINT value;
		SYMENT ent;
		unsigned char buff[SYMESZ];
		char *name;

		if (fread(buff, SYMESZ, 1, obj->fp) != 1)
			return -1;
		if (aux > 0) {
			aux--;
			continue;
		}
		getsym(obj, buff, &ent);
		aux = ent.n_numaux;
		name = symname(obj, &ent);
		type = typeof(obj, &ent);
		sym = lookup(name);

		if (ent.n_scnum <= 0)
			continue;

		switch (sym->type) {
		case 'U':
			sym->type = type;
			sym->value = getval(obj, &ent);
			if (type != 'U') {
				obj->define = 1;
				sym->where = obj;
			}
			if (type == 'C')
				sym->size = ent.n_value;
			break;
		case 'C':
			switch (type) {
			case 'U':
			case 'C':
				if (ent.n_value > sym->size)
					sym->size = ent.n_value;
				break;
			default:
				obj->define = 1;
				sym->where =  obj;
				sym->type = type;
				sym->value = getval(obj, &ent);
				break;
			}
			break;
		default:
			if (type != 'U') {
				fprintf(stderr,
				        "ld: %s: redifinition of symbol '%s'\n",
				        obj->fname, sym->name);
			}
			break;
		}

		obj->symbols[i] = sym;
	}

	return 0;
}

static void
load(Obj *obj)
{
	unsigned char buff[FILHSZ];
	FILHDR *hdr;
	char *strtbl;
	long symoff, secoff, stroff, pos;

	pos = ftell(obj->fp);
	if (fread(buff, FILHSZ, 1, obj->fp) != 1)
		goto bad_file;

	if ((hdr = malloc(sizeof(*hdr))) == NULL)
		outmem();
	getfhdr(obj, buff, hdr);
	obj->filhdr = hdr;

	stroff = pos + hdr->f_symptr + hdr->f_nsyms*SYMESZ;
	symoff = pos + hdr->f_symptr;
	secoff = pos + FILHSZ + hdr->f_opthdr;

	if (readstr(obj, stroff) < 0)
		goto bad_file;
	if (readsects(obj, secoff) < 0)
		goto bad_file;
	if (readsyms(obj, symoff) < 0)
		goto bad_file;
	return;

bad_file:
	if (ferror(obj->fp))
		die("ld: %s: %s", obj->fname, strerror(errno));
	die("ld: %s: corrupted file", obj->fname);
}

static void
unload(Obj *obj)
{
	/* TODO */
}

static void
pass1(Obj *obj)
{
	load(obj);
	if (obj->member && !obj->define)
		unload(obj);
		
}

static void
pass2(Obj *obj)
{
}

Fmt coff32;

static Obj *
probe(char *fname, char *member, FILE *fp)
{
	int c;
	int c1, c2;
	fpos_t pos;
	unsigned short magic;
	unsigned align;
	int (*unpack)(unsigned char *, char *, ...);
	Obj *obj;

	fgetpos(fp, &pos);
	c1 = getc(fp);
	c2 = getc(fp);
	fsetpos(fp, &pos);

	if (ferror(fp))
		die("ld: %s: %s", fname, strerror(errno));

	if (c1 == EOF || c2 == EOF)
		return 0;
	magic = c1 | c2 << 8;

	switch (magic) {
	case COFF_I386MAGIC:
	case COFF_Z80MAGIC:
		unpack = lunpack;
		align = 2;
		break;
	default:
		return NULL;
	}

	obj = newobj(fname, member);
	obj->fp = fp;
	obj->unpack = unpack;
	obj->align = align;
	obj->fmt = &coff32;

	return obj;
}

Fmt coff32 = {
	.probe = probe,
	.pass1 = pass1,
	.pass2 = pass2,
};
