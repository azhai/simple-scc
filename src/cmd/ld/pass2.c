#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

#include <scc/mach.h>

#include "ld.h"

static void
mksecs(void)
{
	Objlst *lp;
	Objsec *sp;
	Section *sec;

	for (lp = objhead; lp; lp = lp->next) {
		for (sp = lp->obj->secs; sp; sp = sp->next) {
			sec = section(sp->name);
			if (sec->type == '?') {
				sec->type = sp->type;
				sec->flags = sp->flags;
			}

			if (sec->type != sp->type || sec->flags != sp->flags) {
				error("incompatible definitions of section '%s'",
				      sec->name);
			}

			/* TODO: use add symbol aligment */
			sec->size += sp->size;
		}
	}
}

static void
merge(Segment *seg)
{
	Section *sec, **p;
	int n = 0;

	for (sec = sechead; sec; sec = sec->next) {
		if (sec->type != seg->type)
			continue;
		p = realloc(seg->sections, (n+1) * sizeof(*p));
		if (!p) {
			error("ou of memory");
			exit(EXIT_FAILURE);
		}
		p[n++] = sec;
		seg->sections = p;
		seg->size += sec->size;
	}

	seg->nsec = n;
}

static void
mksegs(void)
{
	merge(&text);
	merge(&rodata);
	merge(&data);
	merge(&bss);
}

void
pass2(int argc, char *argv[])
{
	unsigned long long n;
	char *end;

	mksecs();
	mksegs();

	if (Dflag) {
		n = strtoull(Dflag, &end, 0);
		if (n == ULLONG_MAX || *end != '\0') {
			error("incorrect -D value");
			exit(EXIT_FAILURE);
		}

		if (n > data.size)
			data.size = n;
	}
}
