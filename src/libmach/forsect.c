#include <stdio.h>

#include <scc/mach.h>

#include "libmach.h"

int
forsect(Obj *obj, int (*fn)(Objsect *, void *), void *data)
{
	int i, r;

	for (i = 0; i < obj->nsecs; i++) {
		r = (*fn)(&obj->sections[i], data);
		if (r <= 0)
			return r;
	}

	return 1;
}
