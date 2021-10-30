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
	.del = elf64del,
	.write = NULL,
	.getsym = NULL,
	.getsec = NULL,
	.loadmap = NULL,
};

char *
elf64str(Obj *obj, int n, long stroff)
{
	char *tbl;
	size_t siz;
	struct elf64 *elf;

	elf = obj->data;

	tbl = elf->strtbl[n];
	siz = elf->strsiz[n];

	if (!tbl || siz < stroff)
		return "";
	return &tbl[stroff];
}
