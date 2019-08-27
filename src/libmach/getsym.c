#include <stdio.h>

#include <scc/mach.h>

Symbol *
getsym(Obj *obj, long *index, Symbol *sym)
{
	return (*obj->ops->getsym)(obj, index, sym);
}
