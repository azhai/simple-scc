#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <scc/mach.h>
#include <scc/cstd.h>

#include "../libmach.h"
#include "coff32.h"

int
coff32getidx(int order, long *nsyms, Objsymdef **def, FILE *fp)
{
	int j, c;
	long i, n;
	char *s;
	Objsymdef *bp;
	unsigned char buf[EXTIDENTSIZ];

	if (fread(buf, 4, 1, fp) != 1)
		return -1;
	unpack(order, buf, "l", &n);

	if (n <= 0)
		return -1;

	if ((bp = calloc(sizeof(*bp), n)) == NULL)
		return -1;

	for (i = 1; i < n-1; i++)
		bp[i].next = &bp[i-1];

	for (i = 0; i < n; i++) {
		fread(buf, 4, 1, fp);
		unpack(order, buf, "l", &bp[i].offset);
	}

	for (i = 0; i < n; i++) {
		for (j = 0; (c = getc(fp)) != EOF && c != '\0'; j++)
			buf[j] = c;
		buf[j++] = '\0';

		if ((s = malloc(j)) == NULL)
			goto error;
		memcpy(s, buf, j);
		bp[i].name = s;
	}

	if (ferror(fp))
		goto error;

	*nsyms = n;
	*def = bp;

	return 0;

error:
	for (i = 0; i < n; i++)
		free(bp[i].name);
	free(bp);
	return -1;
}
