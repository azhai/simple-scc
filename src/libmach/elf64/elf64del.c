#include <stdio.h>
#include <stdlib.h>

#include <scc/mach.h>

#include "../libmach.h"
#include "elf64.h"

void
elf64del(Obj *obj)
{
        struct elf64 *elf = obj->data;

        if (elf) {
                free(elf->phdr);
                free(elf->shdr);
		free(elf->syms);
                free(elf->strtbl[0]);
                free(elf->strtbl[1]);
        }
        free(obj->data);
        obj->data = NULL;
}
