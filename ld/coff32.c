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
	}

	return 0;
}

static void
getent(Obj *obj, unsigned char *buff, SYMENT *ent)
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

static int
readents(Obj *obj, long off)
{
	SYMENT *ent, *ents;
	FILHDR *hdr = obj->filhdr;;
	long i, nsyms = hdr->f_nsyms;
	unsigned char buff[SYMESZ];


	if (fseek(obj->fp, off, SEEK_SET) == EOF)
		return -1;

	if (nsyms > SIZE_MAX/sizeof(SYMENT)) {
		fprintf(stderr,
		        "ld: %s: overflow in size of symbol redirection\n",
		        obj->fname);
		exit(EXIT_FAILURE);
	}

	if ((ents = malloc((nsyms * sizeof(SYMENT)))) == NULL)
		outmem();
	obj->enthdr = ents;

	for (ent = ents; ent < &ents[nsyms]; ++ent) {
		if (fread(buff, SYMESZ, 1, obj->fp) != 1)
			return -1;
		getent(obj, buff, &ents[i]);
	}

	return 0;
}

/*
 * check overflow in: off + ptr + nitem*size
 */
static long
fileptr(long off, long ptr, long nitem, long size)
{
	if (off < 0 || ptr < 0 || nitem < 0 || size < 0)
		return -1;

	if (off > LONG_MAX - ptr)
		return -1;
	off += ptr;

	if (size > 0) {
		if (nitem > LONG_MAX / size)
			return -1;
		size *= nitem;
	}

	if (off > LONG_MAX - size)
		return -1;
	off += size;

	return off;
}

static void
readobj(Obj *obj)
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

	stroff = fileptr(pos, hdr->f_symptr, hdr->f_nsyms, SYMESZ);
	symoff = fileptr(pos, hdr->f_symptr, 0, 0);
	secoff = fileptr(pos, hdr->f_opthdr, 1, FILHSZ);

	if (stroff < 0 || symoff < 0 || secoff < 0)
		goto bad_file;

	if (readstr(obj, stroff) < 0)
		goto bad_file;
	if (readsects(obj, secoff) < 0)
		goto bad_file;
	if (readents(obj, symoff) < 0)
		goto bad_file;
	return;

bad_file:
	fprintf(stderr,
	        "ld: %s: %s\n",
	         obj->fname,
	         (ferror(obj->fp)) ? strerror(errno) : "corrupted file");
	exit(EXIT_FAILURE);
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

static int
needed(Obj *obj)
{
	FILHDR *hdr = obj->filhdr;
	SYMENT *ent, *ents = obj->enthdr;
	long aux, i;

	aux = 0;
	for (i = 0; i < hdr->f_nsyms; i++) {
		if (aux > 0) {
			aux--;
			continue;
		}
		ent = ents + i;
		if (ent->n_sclass != C_EXT)
			continue;

		switch (ent->n_scnum) {
		case N_DEBUG:
		case N_UNDEF:
			continue;
		case N_ABS:
		default:
			if (!lookup(symname(obj, ent), NOINSTALL))
				continue;
			return 1;
		}
	}

	return 0;
}

static void
pass1(Obj *obj)
{
	readobj(obj);

	if (obj->member) {
		if (!needed(obj)) {
			delobj(obj);
			return;
		}
	}

	add(obj);
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

	obj = newobj(fname, member, fp);
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
