#include <stdio.h>

#include <scc/mach.h>

#include "libmach.h"

Section *
getsec(Obj *obj, long *idx, Section *sec)
{
	return (*obj->ops->getsec)(obj, idx, sec);
}
