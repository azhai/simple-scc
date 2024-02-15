#include <stdio.h>

#include <scc/mach.h>

#include "../libmach.h"
#include "elf64.h"

struct arch elf64archs[] = {
	{
		.name = "elf64-amd64",
		.mach = EM_X86_64,
		.endian = ELFDATA2LSB,
		.type = OBJ(ELF64, ARCHAMD64, LITTLE_ENDIAN),
	},
	NULL,
};
