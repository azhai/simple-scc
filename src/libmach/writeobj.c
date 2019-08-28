#include <stdio.h>

#include <scc/mach.h>

#include "libmach.h"

int
writeobj(Obj *obj, FILE *fp)
{
	return (obj->ops->write)(obj, fp);
}