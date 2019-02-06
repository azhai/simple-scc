#include <stdio.h>

#include <scc/mach.h>

#include "libmach.h"

int
objpos(Obj *obj, FILE *fp, long pos)
{
	if (fsetpos(fp, &obj->pos))
		return 0;
	if (fseek(fp, pos, SEEK_CUR) < 0)
		return 0;
	return 1;
}
