#include <stdio.h>

#include <scc/mach.h>

#include "libmach.h"

extern struct objfmt coff32;

struct objfmt *objfmts[] = {
	[COFF32] = &coff32,
	[NFORMATS] = NULL,
};
