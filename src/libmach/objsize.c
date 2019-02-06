#include <limits.h>
#include <stdio.h>

#include <scc/mach.h>

#include "libmach.h"

int
objsize(Obj *obj,
        unsigned long long *text,
        unsigned long long *data,
        unsigned long long *bss)
{
	Section *sp, *secs = obj->sections;
	unsigned long long *p;

	*text = 0;
	*data = 0;
	*bss = 0;
	for (sp =secs; sp < &secs[obj->nsecs]; sp++) {
		switch (sp->type) {
		case 'T':
			p = text;
			break;
		case 'D':
			p = data;
			break;
		case 'B':
			p = bss;
			break;
		default:
			continue;
		}

		if (*p > ULLONG_MAX - sp->size)
			return -1;
		*p += sp->size;
	}
	return 0;
}
