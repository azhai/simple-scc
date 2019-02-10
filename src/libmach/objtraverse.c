#include <stdio.h>

#include <scc/mach.h>

#include "libmach.h"

int
objtraverse(Obj *obj, int (*fn)(Objsym *, void *), void *data)
{
	Objsym *sym;

	for (sym = obj->head; sym; sym = sym->next) {
		 if (!(*fn)(sym, data))
			return 0;
	}
	return 1;
}
