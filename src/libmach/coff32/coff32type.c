#include <stdio.h>
#include <string.h>

#include <scc/mach.h>

#include "../libmach.h"
#include "coff32.h"

int
coff32type(char *name)
{
	struct arch *ap;

	for (ap = coff32archs; ap->name; ap++) {
		if (strcmp(ap->name, name) == 0)
			return ap->type;
	}

	return -1;
}
