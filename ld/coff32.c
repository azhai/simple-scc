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
static long strtbl, symtbl, sectbl;

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

static void
pass1(char *fname, char *member, FILE *fp)
{
	unsigned char buff[FILHSZ];
	FILHDR *hdr;
	Obj *obj;
	long siz, pos = ftell(fp);
	char *str;

	if (fread(buff, FILHSZ, 1, fp) != 1)
		goto bad_file;

	if ((hdr = malloc(sizeof(*hdr))) == NULL)
		goto out_of_memory;

	obj = newobj(fname);
	obj->hdr = getfhdr(buff, hdr);

	/* TODO: Check overflow */
	strtbl = pos + hdr->f_symptr + hdr->f_nsyms* SYMESZ;
	symtbl = pos + hdr->f_symptr;
	sectbl = pos + FILHSZ + hdr->f_opthdr;

	if (fseek(fp, strtbl, SEEK_SET) == EOF)
		goto bad_file;

	if (fread(buff, 4, 1, fp) != 1)
		goto bad_file;

	(*unpack)(buff, "l", &siz);

	if (siz > SIZE_MAX || (str = malloc(siz)) == NULL)
		goto out_of_memory;

	if (fread(str, siz, 1, fp) != 1)
		goto bad_file;

	obj->strtbl = str;

out_of_memory:
	die("ld: out of memory");
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
