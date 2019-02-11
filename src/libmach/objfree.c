#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <scc/mach.h>

#include "libmach.h"

extern delfun_t delv[];

int
objfree(Obj *obj, int what)
{
	int fmt;
	delfun_t fn;

	if (what & TARGETDEL) {
		fmt = FORMAT(obj->type);
		if (fmt < NFORMATS)
			return -1;
		fn = delv[fmt];
		(*fn)(obj);
	}

	if (what & GENERICDEL) {
		free(obj->symbols);
		obj->symbols = NULL;
		memset(obj->htab, 0, sizeof(obj->htab));
	}

	return 0;
}
