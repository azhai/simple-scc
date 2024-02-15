#include <stdio.h>

#include <scc/mach.h>

#include "libmach.h"

int
objtype(char *name)
{
	int t;
	Objops **opsp, *ops;

	for (opsp = objops; ops = *opsp; ++opsp) {
		t = (*ops->type)(name);
		if (t < 0)
			continue;
		return t;
	}

	return -1;
}
