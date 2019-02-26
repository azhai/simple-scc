#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

#include <scc/mach.h>

#include "ld.h"

static unsigned long long
sectsize(int type)
{
	unsigned long long size;
	Objlst *lp;
	Objsect *sp;

	size = 0;
	for (lp = objhead; lp; lp = lp->next) {
		for (sp = lp->obj->secs; sp; sp = sp->next) {
			if (sp->type != type)
				continue;
			size += sp->size;
		}
	}

	return size;
}

/*
 * calculate the size of every segment
 */
void
pass2(int argc, char *argv[])
{
	unsigned long long n;
	char *end;
	Objsect *sp;

	datasiz = bsssiz = textsiz = 0;

	textsiz = sectsize('T');
	datasiz = sectsize('D');
	bsssiz = sectsize('B');

	if (Dflag) {
		n = strtoull(Dflag, &end, 0);
		if (n == ULLONG_MAX || *end != '\0') {
			error("incorrect -D value");
			exit(EXIT_FAILURE);
		}
		if (n > datasiz)
			datasiz = n;
	}
}
