#include <stdio.h>

#include <scc/mach.h>

#include "libmach.h"

extern probefun_t probev[];

int
objtype(FILE *fp, char **name)
{
	int n, i;
	probefun_t *fn;
	fpos_t pos;
	unsigned char buf[NBYTES];

	fgetpos(fp, &pos);
	n = fread(buf, NBYTES, 1, fp);
	fsetpos(fp, &pos);

	if (n != 1 || ferror(fp))
		return -1;

	for (fn = probev; fn < &probev[NFORMATS]; ++fn) {
		n = (*fn)(buf, name);
		if (n == -1)
			continue;
		return n;
	}

	return -1;
}
