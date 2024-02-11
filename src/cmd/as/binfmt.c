#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <scc/scc.h>

#include "as.h"

void
writeout(char *fname)
{
	Section *sp;
	FILE *fp;

	if ((fp = fopen(fname, "wb")) == NULL)
		goto error;

	for (sp = seclist; sp; sp = sp->next) {
		if (!sp->mem)
			continue;
		fwrite(sp->mem, sp->max - sp->base, 1, fp);
	}

	if (fclose(fp))
		goto error;
	outfile = NULL;

	return;

error:
	fprintf(stderr, "as: %s: %s\n", fname, strerror(errno));
	exit(EXIT_FAILURE);
}

