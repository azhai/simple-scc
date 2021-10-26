#include <stdio.h>

#include <scc/mach.h>

#include "../libmach.h"
#include "elf64.h"

struct objops coff32 = {
	.probe = elf64probe,
	.new = elf64new,
	.read = NULL,
	.getidx = NULL,
	.setidx = NULL,
	.pc2line = NULL,
	.strip = NULL,
	.del = NULL,
	.write = NULL,
	.getsym = NULL,
	.getsec = NULL,
	.loadmap = NULL,
};
