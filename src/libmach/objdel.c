#include <stdio.h>
#include <stdlib.h>

#include <scc/mach.h>

#include "libmach.h"

void
objdel(Obj *obj)
{
	objfree(obj, TARGETDEL | GENERICDEL);
	free(obj);
}
