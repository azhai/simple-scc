#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <scc/mach.h>

#include "libmach.h"

static int (*funv[])(Obj *) = {
	[COFF32] = coff32new,
};

Obj *
objnew(int type)
{
	Obj *obj;
	int fmt;

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

	if ((*funv[fmt])(obj) < 0) {
		free(obj);
		return NULL;
	}

	return obj;
}
