#include <stdio.h>

#include <scc/mach.h>

#include "libmach.h"

static int (*funv[])(unsigned char *, char **) = {
	[COFF32] = coff32probe,
};

int
objtype(FILE *fp, char **name)
{
	int n, i;
	int (**fn)(unsigned char *, char **);
	fpos_t pos;
	unsigned char buf[NBYTES];

	fgetpos(fp, &pos);
	n = fread(buf, NBYTES, 1, fp);
	fsetpos(fp, &pos);

	if (n != 1 || ferror(fp))
		return -1;

	for (fn = funv; fn < &funv[NFORMATS]; ++fn) {
		n = (**fn)(buf, name);
		if (n == -1)
			continue;
		return n;
	}

	return -1;
}
