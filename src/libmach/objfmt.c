#include <stdio.h>

#include <scc/mach.h>
#include "libmach.h"

extern struct format objcoff32;

struct format *objfmt[] = {
	[COFF32] = &objcoff32,
	[NFORMATS] = NULL,
};
