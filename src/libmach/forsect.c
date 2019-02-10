#include <stdio.h>

#include <scc/mach.h>

#include "libmach.h"

int
forsect(Obj *obj, int (*fn)(Objsect *, void *), void *data)
{
	int i;

	for (i = 0; i < obj->nsecs; i++) {
		if ((*fn)(&obj->sections[i], data))
			return 0;
	}

	return 1;
}
