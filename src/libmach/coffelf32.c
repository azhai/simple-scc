#include <stdio.h>
#include <string.h>

#include <scc/mach.h>

#include "libmach.h"

long
coff32idx(int order, long nsyms, Objsymdef *head, FILE *fp)
{
	long i, n;
	size_t len;
	Objsymdef *def;
	unsigned char buff[4];

	pack(order, buff, "l", nsyms);
	fwrite(buff, 4, 1, fp);
	n = 4;

	for (def = head; def; def = def->next) {
		pack(order, buff, "l", (long) def->offset);
		fwrite(buff, 4, 1, fp);
		n += 4;
	}

	for (def = head; def; def = def->next) {
		len = strlen(def->name);
		fwrite(def->name, len+1, 1, fp);
		n += len;
	}

	return fflush(fp) == EOF ? -1 : n;
}
