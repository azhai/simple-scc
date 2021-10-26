#include <stdio.h>

#include <scc/mach.h>

#include "../libmach.h"
#include "elf64.h"

struct objops elf64 = {
	.probe = elf64probe,
	.new = elf64new,
	.read = elf64read,
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
