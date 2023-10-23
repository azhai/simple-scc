#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <scc/mach.h>

#include "../libmach.h"
#include "elf64.h"

static int
typeof(Elf64 *elf, Elf_Sym *ent, char *name)
{
	int c, bind;
	unsigned long flags, type;
	Elf_Shdr *shdr;

	switch (ent->st_shndx) {
	case SHN_UNDEF:
		c = 'U';
		break;
	case SHN_ABS:
		c = 'a';
		break;
	case SHN_COMMON:
		c = 'C';
		break;
	case SHN_XINDEX:
		abort();
	default:
		shdr = &elf->shdr[ent->st_shndx];
		flags = shdr->sh_flags;
		type = shdr->sh_type;

		if (flags & SHF_ALLOC) {
			if (type == SHT_NOBITS)
				c = 'b';
			else if (flags & SHF_WRITE)
				c = 'd';
			else if (flags & SHF_EXECINSTR)
				c = 't';
			else
				c = 'r';
		} else if (strncmp(name, ".debug", 6) == 0) {
			c = 'N';
		} else if (strcmp(name, ".comment") == 0) {
			c = 'N';
		} else if (strcmp(name, ".line") == 0) {
			c = 'N';
		} else if (strcmp(name, ".stab") == 0) {
			c = 'N';
		} else {
			c = '?';
		}
	}

	if (ELF_ST_BIND(ent->st_info) != STB_LOCAL)
		c = toupper(c);

	return c;
}

static int
stypeof(Elf_Sym *ent)
{
	switch (ELF_ST_TYPE(ent->st_info)) {
	case STT_OBJECT:
		return SYMOBJECT;
	case STT_FUNC:
		return SYMFUNC;
	case STT_SECTION:
		return SYMSECTION;
	case STT_FILE:
		return SYMFILE;
	case STT_COMMON:
		return SYMCOMMON;
	default:
	case STT_NOTYPE:
		return SYMNOTYPE;
	}
}

Symbol *
elf64getsym(Obj *obj, int *idx, Symbol *sym)
{
	int n = *idx;
	Elf_Sym *ent;
	Elf64 *elf = obj->data;

	if (n == 0)
		n++;

	if (!elf->symtab || n >= elf->nsym)
		return NULL;
	ent = &elf->syms[n];

	if (ELF_ST_TYPE(ent->st_info) == STT_SECTION) {
		Elf_Shdr *shdr = &elf->shdr[ent->st_shndx];
		sym->name = elf64str(obj, SEC_STRTBL, shdr->sh_name);
	} else {
		sym->name = elf64str(obj, SYM_STRTBL, ent->st_name);
	}

	sym->type = typeof(elf, ent, sym->name);
	sym->stype = stypeof(ent);
	sym->value = ent->st_value;
	sym->size = ent->st_size;
	sym->index = *idx = n;

	return sym;
}
