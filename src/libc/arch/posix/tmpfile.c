#include <stdio.h>

#include "../../syscall.h"

#undef tmpfile

FILE *
tmpfile(void)
{
	char *fname;
	FILE *fp;

	for (;;) {
		if ((fname = tmpnam(NULL)) == NULL)
			return NULL;
		if ((fp = fopen(fname, "wt+")) == NULL)
			continue;
		_unlink(fname);
		return fp;
	}
}
