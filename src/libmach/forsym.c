#include <stdio.h>

#include <scc/mach.h>

#include "libmach.h"

int
forsym(Obj *obj, int (*fn)(Objsym *, void *), void *data)
{
	int r;
	Objsym *sym;

	for (sym = obj->symbols; sym; sym = sym->next) {
		r = (*fn)(sym, data);
		if (r <= 0)
			return r;
	}
	return 1;
}
