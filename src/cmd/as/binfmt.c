#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <scc/mach.h>
#include <scc/scc.h>

#include "as.h"

/*
 * FIXME: Horrible hack, just the data structure copied here
 * To be able to continue working and not breaking the tests
 * but it should be removed from here as soon as posible.
 */
struct lsection {
	Section sec;
	FILE *fp;
};

static int
dumpsec(Section *sec, void *dst)
{
	int c;
	struct lsection *lsec = (struct lsection *)  sec;
	FILE *src = lsec->fp;

	if (!src)
		return 0;

	rewind(src);
	while ((c = getc(src)) != EOF)
		putc(c, dst);

	if (ferror(src))
		return -1;

	return 0;
}

void
writeout(char *fname)
{
	FILE *fp;

	if ((fp = fopen(fname, "wb")) == NULL)
		goto error;

	if (forallsecs(dumpsec, fp) < 0)
		goto error;
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
