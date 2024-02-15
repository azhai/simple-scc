#include <stdio.h>

#include <scc/mach.h>

#include "../libmach.h"
#include "elf64.h"

int
elf64probe(unsigned char *buf, char **name)
{
	int endian;
	Elf_Ehdr hdr;
	struct arch *ap;

	unpack(buf[EI_DATA] == ELFDATA2LSB ? LITTLE_ENDIAN : BIG_ENDIAN,
	       buf,
	       "'16sss",
	       hdr.e_ident,
	       &hdr.e_type,
	       &hdr.e_machine,
	       &hdr.e_version);

	if (!IS_ELF(hdr)
	||  buf[EI_CLASS] != ELFCLASS64
	||  buf[EI_DATA] == ELFDATANONE
	||  buf[EI_VERSION] != EV_CURRENT
	||  (buf[EI_DATA] != ELFDATA2LSB && buf[EI_DATA] != ELFDATA2MSB)) {
		return -1;
	}

	if (hdr.e_version != EV_CURRENT)
		return -1;

	endian = hdr.e_ident[EI_DATA];
	for (ap = elf64archs; ap->name; ap++) {
		if (ap->mach == hdr.e_machine &&  ap->endian == endian) {
			if (name)
				*name = ap->name;
			return ap->type;
		}
	}

	return -1;
}
