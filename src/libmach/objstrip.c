#include <stdio.h>

#include <scc/mach.h>

#include "libmach.h"

extern stripfun_t stripv[];

/* TODO: It is better to move this to a common place */
static void
delsyms(Obj *obj)
{
	Symbol *sym, *next;

	for (sym = obj->head; sym; sym = next) {
		next = sym->next;
		free(sym->name);
		free(sym);
	}

	obj->head = NULL;
	memset(obj->htab, 0, sizeof(obj->htab));
}

int
objstrip(Obj *obj)
{
	int fmt;
	stripfun_t fn;

	fmt = FORMAT(obj->type);
	if (fmt >= NFORMATS)
		return -1;
	fn = stripv[fmt];
	(*fn)(obj);
	delsyms(obj);
	return 0;
}
