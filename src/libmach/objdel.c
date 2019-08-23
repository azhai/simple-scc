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

	obj->syms = NULL;
	obj->secs = NULL;
	memset(obj->htab, 0, sizeof(obj->htab));
}
