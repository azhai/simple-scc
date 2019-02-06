#include <stdio.h>
#include <stdlib.h>

#include <scc/mach.h>

#include "libmach.h"

int
objdel(Obj *obj)
{
	if (objreset(obj) < 0)
		return -1;
	free(obj);
}
