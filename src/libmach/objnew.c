#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <scc/mach.h>

#include "libmach.h"

Obj *
objnew(int type)
{
	Obj *obj;
	int fmt;
	Objops *ops;

	fmt = FORMAT(type);
	if (fmt >= NFORMATS)
		return NULL;

	if ((obj = malloc(sizeof(*obj))) == NULL)
		return NULL;

	obj->type = type;
	obj->syms = NULL;
	obj->secs = NULL;
	obj->nsyms = 0;
	obj->nsecs = 0;
	memset(obj->htab, 0, sizeof(obj->htab));

	ops = objops[fmt];
	obj->ops = ops;

	if ((*ops->new)(obj) < 0) {
		free(obj);
		return NULL;
	}

	return obj;
}
