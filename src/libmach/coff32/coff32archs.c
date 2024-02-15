#include <stdio.h>

#include <scc/mach.h>

#include "../libmach.h"
#include "coff32.h"

struct arch coff32archs[] = {
	"coff32-i386", "\x4c\x01", OBJ(COFF32, ARCH386, LITTLE_ENDIAN),
	"coff32-z80", "\x5a\x80", OBJ(COFF32, ARCHZ80, LITTLE_ENDIAN),
	NULL,
};
