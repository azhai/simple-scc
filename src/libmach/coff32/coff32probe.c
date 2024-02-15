#include <stdio.h>

#include <scc/mach.h>

#include "../libmach.h"
#include "coff32.h"

int
coff32probe(unsigned char *buf, char **name)
{
	struct arch *ap;

	for (ap = coff32archs; ap->name; ap++) {
		if (ap->magic[0] == buf[0] && ap->magic[1] == buf[1]) {
			if (name)
				*name = ap->name;
			return ap->type;
		}
	}
	return -1;
}
