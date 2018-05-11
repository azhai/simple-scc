static char sccsid[] = "@(#) ./ld/coff32.c";

#include <assert.h>
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../inc/coff32/filehdr.h"
#include "../inc/coff32/scnhdr.h"
#include "../inc/coff32/syms.h"
#include "../inc/scc.h"
#include "ld.h"

static int (*unpack)(unsigned char *, char *, ...);

static FILHDR *
getfhdr(unsigned char *buff, FILHDR *hdr)
{
	int n;

	n = (*unpack)(buff,
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

	(*unpack)(buff, "l", &siz);

	if (siz > SIZE_MAX || (str = malloc(siz)) == NULL)
		outmem();

	if (fread(str, siz, 1, obj->fp) != 1)
		return -1;

	obj->strtbl = str;
	return 0;
}

static SCNHDR *
getscn(unsigned char *buff, SCNHDR *scn)
{
	int n;

	n = (*unpack)(buff,
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
	unsigned i;
	unsigned char buff[SCNHSZ];
	SCNHDR scn;
	FILHDR *hdr;

	if (fseek(obj->fp, off, SEEK_SET) == EOF)
		return -1;

	hdr = obj->filhdr;
	for (i = 0; i < hdr->f_nscns; i++) {
		if (fread(buff, SCNHSZ, 1, obj->fp) != 1)
			return -1;
		getscn(buff, &scn);
	}
}

static void
getsym(unsigned char *buff, SYMENT *ent)
{
	int n;
	long off, zero;
	char *name;

	n = (*unpack)(buff,
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
		(*unpack)(buff, "ll", &ent->n_zeroes, &ent->n_offset);
}

static int
loadobj(Obj *obj, long off)
{
	unsigned i;
	unsigned char buff[SYMESZ];
	SYMENT sym;
	FILHDR *hdr;

	for (i = 0; i < hdr->f_nsyms; i++) {
		if (fread(buff, SYMESZ, 1, obj->fp) != 1)
			return -1;
		getsym(buff, &sym);
	}
}

static void
pass1(char *fname, char *member, FILE *fp)
{
	unsigned char buff[FILHSZ];
	FILHDR *hdr;
	Obj *obj;
	char *strtbl;
	long symoff, secoff, stroff, pos;

	obj = newobj(fname, member);
	obj->fp = fp;

	pos = ftell(fp);
	if (fread(buff, FILHSZ, 1, fp) != 1)
		goto bad_file;

	if ((hdr = malloc(sizeof(*hdr))) == NULL)
		outmem();
	getfhdr(buff, hdr);
	obj->filhdr = hdr;

	/* TODO: Check overflow */
	stroff = pos + hdr->f_symptr + hdr->f_nsyms*SYMESZ;
	symoff = pos + hdr->f_symptr;
	secoff = pos + FILHSZ + hdr->f_opthdr;

	if (readstr(obj, stroff) < 0)
		goto bad_file;
	if (loadobj(obj, symoff) < 0)
		goto bad_file;
	if (readsects(obj, secoff) < 0)
		goto bad_file;

bad_file:
	if (ferror(fp))
		die("ld: %s: %s", fname, strerror(errno));
	die("ld: %s: corrupted file", fname);
}

static void
pass2(char *fname, char *member, FILE *fp)
{
}

static int
probe(char *fname, char *member, FILE *fp)
{
	int c;
	int c1, c2;
	fpos_t pos;
	unsigned short magic;

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
	case COFF_Z80MAGIC:
		return 1;
	default:
		return 0;
	}
}

struct objfile coff32 = {
	.probe = probe,
	.pass1 = pass1,
	.pass2 = pass2,
};
