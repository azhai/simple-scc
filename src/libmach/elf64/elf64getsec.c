#include <stdio.h>

#include <scc/mach.h>

#include "../libmach.h"
#include "elf64.h"

Section *
elf64getsec(Obj *obj, int *idx, Section *sec)
{
	int stype, n = *idx;
	unsigned long flags, type;
	unsigned sflags;
	Elf64 *elf = obj->data;
	Elf_Ehdr *hdr = &elf->hdr;
	Elf_Shdr *shdr;

	if (n >= elf->nsec)
		return NULL;

	shdr = &elf->shdr[n];
	flags = shdr->sh_flags;
	type = shdr->sh_type;

	if (flags & SHF_ALLOC) {
		if (type == SHT_NOBITS)
			stype = 'B';
		else if (flags & SHF_WRITE)
			stype = 'D';
		else
			stype = 'T';
	} else {
		stype = 'N';
	}

	sflags = 0;
	if (flags & SHF_WRITE)
		sflags |= SWRITE;
	if (flags  & SHF_EXECINSTR)
		sflags |= SEXEC;
	if (flags & SHF_ALLOC)
		sflags |= SALLOC|SREAD;
	if (type != SHT_NOBITS)
		sflags |= SLOAD;
	if (flags & SHF_MERGE)
		sflags |= SSHARED; /* TODO: Check SSHARED in coff */
	if (stype == 'T' || stype == 'D')
		sflags |= SRELOC;  /* TODO: Check SRELOC in coff */

	sec->name = elf64str(obj, SEC_STRTBL, shdr->sh_name);
	sec->index = n;
	sec->size = shdr->sh_size;
	sec->base = shdr->sh_addr;
	sec->type = stype;
	sec->flags = sflags;
	sec->align = shdr->sh_addralign;

	return sec;
}
