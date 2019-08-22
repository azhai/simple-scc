#include <stdio.h>

#include <scc/mach.h>

#include "../libmach.h"
#include "coff32.h"

struct objfmt coff32 = {
	.new = coff32new,
	.read = coff32read,
};
