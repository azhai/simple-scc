#include <stdio.h>

#include <scc/mach.h>

#include "libmach.h"

Symbol *
setsym(Obj *obj, int *index, Symbol *sym)
{
	return (*obj->ops->setsym)(obj, index, sym);
}
