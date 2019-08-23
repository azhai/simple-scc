#include <stdio.h>

#include <scc/mach.h>

#include "../libmach.h"
#include "coff32.h"

Objops coff32 = {
	.new = coff32new,
	.read = coff32read,
	.getidx = coff32getidx,
	.setidx = coff32setidx,
	.addr2line = coff32addr2line,
	.strip = coff32strip,
};
