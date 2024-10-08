#include <stdio.h>

#include <scc/mach.h>

#include "../libmach.h"
#include "coff32.h"

struct objops coff32 = {
	.type = coff32type,
	.probe = coff32probe,
	.new = coff32new,
	.read = coff32read,
	.getidx = coff32getidx,
	.setidx = coff32setidx,
	.pc2line = coff32pc2line,
	.strip = coff32strip,
	.del = coff32del,
	.write = coff32write,
	.getsym = coff32getsym,
	.setsym = coff32setsym,
	.getsec = coff32getsec,
	.setsec = coff32setsec,
	.loadmap = coff32loadmap,
};
