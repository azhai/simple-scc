#include <stdio.h>

#include <scc/ar.h>
#include <scc/mach.h>

int
formember(FILE *fp, int (*fn)(FILE *, char *, void *), void *data)
{
	int r;
	long off;
	fpos_t pos;
	char name[SARNAM+1];

	for (;;) {
		fgetpos(fp, &pos);

		if ((off = armember(fp, name)) <= 0)
			return off;
		r = (*fn)(fp, name, data);
		if (r <= 0)
			return r;

		fsetpos(fp, &pos);
		fseek(fp, off, SEEK_CUR);
	}
}
