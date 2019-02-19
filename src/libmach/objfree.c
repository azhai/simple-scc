#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <scc/mach.h>

#include "libmach.h"

static void (*funv[])(Obj *) = {
	[COFF32] = coff32del,
};

int
objfree(Obj *obj, int what)
{
	int fmt;

	if (what & TARGETDEL) {
		fmt = FORMAT(obj->type);
		if (fmt < NFORMATS)
			return -1;
		(*funv[fmt])(obj);
	}

	if (what & GENERICDEL) {
		free(obj->secs);
		free(obj->syms);
		obj->syms = NULL;
		memset(obj->htab, 0, sizeof(obj->htab));
	}

	return 0;
}
