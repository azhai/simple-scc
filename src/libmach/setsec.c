#include <stdio.h>

#include <scc/mach.h>

#include "libmach.h"

Section *
setsec(Obj *obj, int *idx, Section *sec)
{
	return (*obj->ops->setsec)(obj, idx, sec);
}
