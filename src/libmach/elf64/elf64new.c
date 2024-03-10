#include <stdio.h>
#include <stdlib.h>

#include <scc/mach.h>

#include "../libmach.h"
#include "elf64.h"

int
elf64new(Obj *obj, int type)
{
	struct elf64 *elf;

	if ((elf = calloc(1, sizeof(*elf))) == NULL)
		return -1;
	obj->data = elf;
	obj->index = "/";
	return 0;
}
