#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include <scc/mach.h>

#include "../libmach.h"
#include "elf64.h"

static int
unpack_hdr(int order, unsigned char *buf, Elf_Ehdr *hdr)
{
	int n;

	n = unpack(order,
	           buf,
	           "'16sslqqqlssssss",
	           hdr->e_ident,
	           &hdr->e_type,
	           &hdr->e_machine,
	           &hdr->e_version,
	           &hdr->e_entry,
	           &hdr->e_phoff,
	           &hdr->e_shoff,
	           &hdr->e_flags,
	           &hdr->e_ehsize,
	           &hdr->e_phentsize,
	           &hdr->e_phnum,
	           &hdr->e_shentsize,
	           &hdr->e_shnum,
	           &hdr->e_shstrndx);

	assert(n == ELFHSZ);

	return n;
}

static int
unpack_phdr(int order, unsigned char *buf, Elf_Phdr *phdr)
{
	int n;

	n = unpack(order,
	           buf,
	           "llqqqqqq",
	           &phdr->p_type,
	           &phdr->p_flags,
	           &phdr->p_offset,
	           &phdr->p_vaddr,
	           &phdr->p_paddr,
	           &phdr->p_filesz,
	           &phdr->p_memsz,
	           &phdr->p_align);

	assert(n == ELFPSZ);

	return n;
}

static int
unpack_shdr(int order, unsigned char *buf, Elf_Shdr *shdr)
{
	int n;

	n = unpack(order,
	           buf,
	           "llqqqqllqq",
	           &shdr->sh_name,
	           &shdr->sh_type,
	           &shdr->sh_flags,
	           &shdr->sh_addr,
	           &shdr->sh_offset,
	           &shdr->sh_size,
	           &shdr->sh_link,
	           &shdr->sh_info,
	           &shdr->sh_addralign,
	           &shdr->sh_entsize);

	assert(n == ELFSSZ);

	return n;
}

static int
unpack_sym(int order, unsigned char *buf, Elf_Sym *sym)
{
	int n;

	n = unpack(order,
	           buf,
	           "lccsqq",
	           &sym->st_name,
	           &sym->st_info,
	           &sym->st_other,
	           &sym->st_shndx,
	           &sym->st_value,
	           &sym->st_size);
	assert(n == ELFESZ);

	return n;
}

static int
readhdr(Obj *obj, FILE *fp)
{
	Elf64 *elf;
	Elf_Ehdr *hdr;
	unsigned char buf[ELFHSZ];

	elf = obj->data;
	hdr = &elf->hdr;

	if (fread(buf, ELFHSZ, 1, fp) != 1)
		return 0;
	unpack_hdr(ORDER(obj->type), buf, hdr);

	switch (hdr->e_type) {
	case ET_REL:
	case ET_EXEC:
	case ET_DYN:
		return 1;
	default:
		return 0;
	}
}

static int
readphdr(Obj *obj, FILE *fp)
{
	long long i;
	Elf_Ehdr *hdr;
	Elf_Phdr *phdr;
	Elf64 *elf;
	unsigned char buf[ELFPSZ];

	elf = obj->data;
	hdr = &elf->hdr;

	if (hdr->e_phoff == 0 || hdr->e_phnum == 0)
		return 1;

	phdr = calloc(hdr->e_phnum, sizeof(*phdr));
	if (!phdr)
		return 0;
	elf->phdr = phdr;

	if (!objpos(obj, fp, hdr->e_phoff))
		return 0;
	for (i = 0; i < hdr->e_phnum; i++) {
		if (fread(buf, ELFPSZ, 1, fp) != 1)
			return 0;
		unpack_phdr(ORDER(obj->type), buf, &phdr[i]);
	}

	return 1;
}

static int
readshdr(Obj *obj, FILE *fp)
{
	unsigned long long i, nsec;
	Elf_Ehdr *hdr;
	Elf_Shdr *shdr;
	Elf64 *elf;
	unsigned char buf[ELFSSZ + ELFHSZ];

	elf = obj->data;
	hdr = &elf->hdr;

	if (hdr->e_shoff == 0)
		return 1;

	if (!objpos(obj, fp, hdr->e_shoff))
		return 0;

	if (hdr->e_shnum != SHN_UNDEF) {
		nsec = hdr->e_shnum;
	} else {
		Elf_Shdr sec0;
		fpos_t pos;

		fgetpos(fp, &pos);
		fread(buf, ELFHSZ, 1, fp);
		fsetpos(fp, &pos);

		if (ferror(fp))
			return 0;

		unpack_shdr(ORDER(obj->type), buf, &sec0);
		nsec = sec0.sh_size;
	}

	if (nsec > SIZE_MAX)
		return 0;

	shdr = calloc(nsec, sizeof(*shdr));
	if (!shdr)
		return 0;
	elf->shdr = shdr;
	elf->nsec = nsec;

	for (i = 0; i < nsec; i++) {
		if (fread(buf, ELFSSZ, 1, fp) != 1)
			return 0;
		unpack_shdr(ORDER(obj->type), buf, &shdr[i]);
		if (shdr[i].sh_type == SHT_SYMTAB) {
			/*
			 * elf supports multiple symbol table, but we don't
			 * care and we only support one, and we reject elf
			 * files with more of one symbol table.
			 */
			if (elf->symtab)
				return 0;
			elf->symtab = &shdr[i];
		}
	}

	return 1;
}

static int
readsecstr(Obj *obj, FILE *fp)
{
	long idx;
	size_t siz;
	char *str;
	Elf_Shdr *shdr;
	Elf64 *elf;
	Elf_Ehdr *hdr;

	elf = obj->data;
	hdr = &elf->hdr;
	idx = hdr->e_shstrndx;
	if (idx == SHN_UNDEF)
		return 0;
	if (idx == SHN_XINDEX) {
		if (hdr->e_shnum == 0)
			return 0;
		idx = elf->shdr[0].sh_link;
	}

	if (idx >= hdr->e_shnum)
		return 0;
	shdr = &elf->shdr[idx];

	if (shdr->sh_size > SIZE_MAX)
		return 0;

	siz = shdr->sh_size;
	if (siz == 0)
		return 1;
	str = malloc(siz);
	if (!str)
		return 0;

	elf->strtbl[SEC_STRTBL] = str;
	elf->strsiz[SEC_STRTBL] = siz;

	if (!objpos(obj, fp, shdr->sh_offset))
		return 0;
	if (fread(str, siz, 1, fp) != 1)
		return 0;

	return 1;
}

static int
readsymstr(Obj *obj, FILE *fp)
{
	long idx;
	size_t siz;
	char *str;
	Elf64 *elf;
	Elf_Shdr *shdr;

	elf = obj->data;
	if (!elf->symtab)
		return 1;

	idx = elf->symtab->sh_link;
	if (idx >= elf->nsec)
		return 0;
	shdr = &elf->shdr[idx];

	if (shdr->sh_size > SIZE_MAX)
		return 0;

	siz = shdr->sh_size;
	if (siz == 0)
		return 1;
	str = malloc(siz);
	if (!str)
		return 0;

	elf->strtbl[SYM_STRTBL] = str;
	elf->strsiz[SYM_STRTBL] = siz;

	if (!objpos(obj, fp, shdr->sh_offset))
		return 0;
	if (fread(str, siz, 1, fp) != 1)
		return 0;

	return 1;
}

static int
readsym(Obj *obj, FILE *fp)
{
	long nsym, i;
	int sec;
	Elf64 *elf;
	Elf_Sym *syms;
	Elf_Shdr *shdr;
	unsigned char buf[ELFSSZ];

	elf = obj->data;
	if (!elf->symtab)
		return 1;
	shdr = elf->symtab;

	assert(shdr->sh_type == SHT_SYMTAB);

	nsym = shdr->sh_size / shdr->sh_entsize;
	if (nsym >= SIZE_MAX)
		return 0;

	syms = calloc(nsym, sizeof(*syms));
	if (!syms)
		return 0;
	elf->syms = syms;
	elf->nsym = nsym;

	if (!objpos(obj, fp, shdr->sh_offset))
		return 0;

	for (i = 0; i < nsym; i++) {
		if (fread(buf, ELFESZ, 1, fp) != 1)
			return 0;
		unpack_sym(ORDER(obj->type), buf, &syms[i]);

		sec = syms[i].st_shndx;
		switch (sec) {
		case SHN_XINDEX:
			/*
			 * Elf supports an extension mechanism to allow
			 * indexes bigger than 4 bytes. We don't care
			 * and we reject elf files using this feature.
			 */
			return 0;
		case SHN_UNDEF:
		case SHN_ABS:
		case SHN_COMMON:
			break;
		default:
			if (sec >= elf->nsec)
				return 0;
			break;
		}
	}

	return 1;
}

int
elf64read(Obj *obj, FILE *fp)
{
	if (!readhdr(obj, fp))
		return -1;
	if (!readphdr(obj, fp))
		return -1;
	if (!readshdr(obj, fp))
		return -1;
	if (!readsym(obj, fp))
		return -1;
	if (!readsecstr(obj, fp))
		return -1;
	if (!readsymstr(obj, fp))
		return -1;

	return 0;
}
