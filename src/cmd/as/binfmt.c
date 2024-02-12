#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <scc/mach.h>
#include <scc/scc.h>

#include "as.h"

static int
dumpsec(Section *sec, void *arg)
{
	FILE *fp = arg;

	if (!sec->mem)
		return 0;

	fwrite(sec->mem, sec->size, 1, fp);
	return 0;
}

void
writeout(char *fname)
{
	FILE *fp;

	if ((fp = fopen(fname, "wb")) == NULL)
		goto error;

	forallsecs(dumpsec, fp);
	fflush(fp);

	if (ferror(fp))
		goto error;

	fclose(fp);
	outfile = NULL;

	return;

error:
	fprintf(stderr, "as: %s: %s\n", fname, strerror(errno));
	exit(EXIT_FAILURE);
}

