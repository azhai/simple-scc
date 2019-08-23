#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <scc/mach.h>

#include "libmach.h"

void
objdel(Obj *obj)
{
	free(obj->secs);
	free(obj->syms);
	free(obj);
}
