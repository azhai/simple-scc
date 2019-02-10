#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <scc/mach.h>

#include "libmach.h"

static void
delsyms(Obj *obj)
{
	Objsym *sym, *next;

	for (sym = obj->head; sym; sym = next) {
		next = sym->next;
		free(sym->name);
		free(sym);
	}

	obj->head = NULL;
	memset(obj->htab, 0, sizeof(obj->htab));
}

static void
delsecs(Obj *obj)
{
	int i;

	for (i = 0; i < obj->nsecs; i++)
		free(obj->sections[i].name);
	free(obj->sections);
	obj->sections = NULL;
}

void
objfree(Obj *obj, int what)
{
	if (what & FREESYM)
		delsyms(obj);
	if (what & FREESECT)
		delsecs(obj);
}
