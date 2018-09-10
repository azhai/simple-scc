static char sccsid[] = "@(#) ./ld/coff32.c";

#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../inc/coff32/aouthdr.h"
#include "../inc/coff32/filehdr.h"
#include "../inc/coff32/scnhdr.h"
#include "../inc/coff32/syms.h"
#include "../inc/coff32/coff32.h"
#include "../inc/scc.h"
#include "ld.h"

#define NUMSCN_MAX 65536
#define NUMENT_MAX 2147483648

static long textpc = 0x1000;

/*
 * check overflow in: off + ptr + nitem*size
 */
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

static int
readsects(Obj *obj, long off)
{
	unsigned a, nsec, i;
	unsigned char buff[SCNHSZ];
	SCNHDR *scns, *p;
	FILHDR *hdr;
	Symbol *sym;
	Section *sp;

	hdr = obj->filhdr;
	nsec = hdr->f_nscns;

	scns = NULL;
	if (nsec <= SIZE_MAX / sizeof(*scns))
		scns = malloc(nsec * sizeof(*scns));
	if (!scns)
		outmem();
	obj->scnhdr = scns;

	if (fseek(obj->fp, off, SEEK_SET) == EOF)
		return -1;

	a = obj->align - 1;
	for (i = 0; i < nsec; ++i) {
		p = &scns[i];
		if (fread(buff, SCNHSZ, 1, obj->fp) != 1)
			return -1;
		coff32_unpack_scn(obj->unpack, buff, p);
		sp = slookup(p->s_name);
		p->s_vaddr = sp->base + sp->size;
		sp->size += p->s_size;
	}

	return 0;
}

static int
readents(Obj *obj, long off)
{
	SYMENT *ent, *ents;
	SCNHDR *scn, *scns = obj->scnhdr;
	FILHDR *hdr = obj->filhdr;;
	long nsyms = hdr->f_nsyms;
	unsigned nsect;
	unsigned char buff[SYMESZ];
	char *s;
	int aux;
	Symbol *sym;


	if (fseek(obj->fp, off, SEEK_SET) == EOF)
		return -1;

	ents = NULL;
	if (nsyms <= SIZE_MAX/sizeof(SYMENT))
		ents = malloc((nsyms * sizeof(SYMENT)));
	if (!ents)
		outmem();
	obj->enthdr = ents;

	aux = 0;
	for (ent = ents; ent < &ents[nsyms]; ++ent) {
		if (fread(buff, SYMESZ, 1, obj->fp) != 1)
			return -1;
		coff32_unpack_ent(obj->unpack, buff, ent);
		s = ent->n_name;
		if (!s[0] && !s[1] && !s[2] && !s[3])
			(*obj->unpack)(buff, "ll", &ent->n_zeroes, &ent->n_offset);

		if (aux > 0) {
			aux--;
			continue;
		}
		aux = ent->n_numaux;

		scn = NULL;
		switch (ent->n_scnum) {
		case N_DEBUG:
			continue;
		case N_ABS:
			break;
		case N_UNDEF:
			/* TODO: deal wth common blocks */
			break;
		default:
			nsect = ent->n_scnum-1;
			if (nsect >= hdr->f_nscns)
				corrupted(obj->fname, obj->member);
			scn = &scns[nsect];
			ent->n_value += scn->s_vaddr;
		}

		if (ent->n_sclass == C_EXT && ent->n_scnum != N_UNDEF) {
			Symbol *sym = lookup(symname(obj, ent), INSTALL);

			if (sym->flags & SDEFINED) {
				redefined(obj, sym);
			} else {
				sym->flags |= SDEFINED;
				sym->where = obj;
				if (scn)
					sym->section = slookup(scn->s_name);
			}
		}
	}

	return 0;
}

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

Obj *
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
	coff32_unpack_hdr(obj->unpack, buff, hdr);
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
	return add(obj);

bad_file:
	fprintf(stderr,
	        "ld: %s: %s\n",
	         obj->fname,
	         (ferror(obj->fp)) ? strerror(errno) : "corrupted file");
	exit(EXIT_FAILURE);
}

Obj *
probe(char *fname, char *member, FILE *fp)
{
	int c;
	int c1, c2;
	long pos;
	unsigned short magic;
	unsigned align;
	int (*unpack)(unsigned char *, char *, ...);
	int (*pack)(unsigned char *, char *, ...);
	Obj *obj;

	pos = ftell(fp);
	c1 = getc(fp);
	c2 = getc(fp);
	fseek(fp, pos, SEEK_SET);

	if (ferror(fp))
		die("ld: %s: %s", fname, strerror(errno));

	if (c1 == EOF || c2 == EOF)
		return 0;
	magic = c1 | c2 << 8;

	switch (magic) {
	case COFF_I386MAGIC:
	case COFF_Z80MAGIC:
		unpack = lunpack;
		pack = lpack;
		align = 2;
		break;
	default:
		return NULL;
	}

	obj = newobj(fname, member, fp);
	obj->unpack = unpack;
	obj->align = align;
	obj->offset = pos;

	return obj;
}

static void
wrhdr(FILE *fp)
{
	FILHDR hdr;
	Section *sp;
	unsigned char buff[FILHSZ];

	if (numsects > NUMSCN_MAX || numsymbols > NUMENT_MAX) {
		fprintf(stderr, "ld: too many symbols or sections\n");
		exit(EXIT_FAILURE);
	}

	/*
	 * we set the timestamp to 0 to make the output
	 * reproductible and to avoid a not standard use
	 * of time()
	 */
	hdr.f_symptr = 0;
	hdr.f_magic = COFF_Z80MAGIC;
	hdr.f_nscns = numsects;
	hdr.f_symptr = 0;
	hdr.f_timdat = 0;
	hdr.f_nsyms = 0;
	hdr.f_opthdr = AOUTSZ;
	hdr.f_flags = F_EXEC | F_AR32WR; /* TODO: set the correct endianess */

	if (!sflag) {
		hdr.f_symptr = 0; /* TODO: set correct value here */
		hdr.f_flags |= F_SYMS;
		hdr.f_nsyms = numsymbols;
	}

	coff32_pack_hdr(lpack, buff, &hdr);
	fwrite(buff, FILHSZ, 1, fp);
}

static void
wraout(FILE *fp)
{
	AOUTHDR aout;
	unsigned char buff[AOUTSZ];
	Symbol *sym;
	long addr;

	if ((sym = lookup(entry, NOINSTALL)) != NULL) {
		addr = sym->value;
	} else {
		fprintf(stderr,
		        "ld: warning: cannot find entry symbol '%s'; defaulting to 0\n",
		        entry);
		addr = 0;
	} 

	aout.magic = ZMAGIC;
	aout.vstamp = 0;
	aout.entry = addr;
	aout.tsize = tsize;
	aout.dsize = dsize;
	aout.bsize = bsize;
	aout.text_start = textpc;
	aout.data_start = textpc + dsize;

	coff32_pack_aout(lpack, buff, &aout);
	fwrite(buff, AOUTSZ, 1, fp);
}

static void
wrscn(FILE *fp, Section *sp, long pc)
{
	SCNHDR scn;
	unsigned char buff[SCNHSZ];

	strcpy(scn.s_name, sp->name);
	scn.s_paddr = pc;
	scn.s_vaddr = pc;
	scn.s_size = sp->size;
	scn.s_scnptr = 0; /* TODO: file ptr */
	scn.s_relptr = 0;
	scn.s_lnnoptr = 0;
	scn.s_nrelloc = 0;
	scn.s_nlnno = 0;
	scn.s_flags = 0; /* TODO: Add flags */

	coff32_pack_scn(lpack, buff, &scn);
	fwrite(buff, SCNHSZ, 1, fp);
}

void
writeout(FILE *fp)
{
	Section *sp;
	long pc = textpc;

	wrhdr(fp);
	wraout(fp);

	for (sp = sectlst; sp; sp = sp->next) {
		wrscn(fp, sp, pc);
		pc += sp->size;
	}

	/* TODO: run over all the files */

	if (fflush(fp) == EOF) {
		perror("ld: error writing output file");
		exit(EXIT_FAILURE);
	}
}
