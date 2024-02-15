#include <stdio.h>

#include <scc/mach.h>

#include "libmach.h"

int
objprobe(FILE *fp, char **name)
{
	int n, t;
	fpos_t pos;
	Objops **opsp, *ops;
	unsigned char buf[NBYTES];

	fgetpos(fp, &pos);
	n = fread(buf, NBYTES, 1, fp);
	fsetpos(fp, &pos);

	if (n != 1 || ferror(fp))
		return -1;

	for (opsp = objops; ops = *opsp; ++opsp) {
		t = (*ops->probe)(buf, name);
		if (t < 0)
			continue;
		return t;
	}

	return -1;
}
