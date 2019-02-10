#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <scc/mach.h>
#include <scc/scc.h>

Objsym *
objlookup(Obj *obj, char *name, int install)
{
	unsigned h;
	size_t len;
	char *s;
	Objsym *sym;

	h = genhash(name) % NR_SYMHASH;
	for (sym = obj->htab[h]; sym; sym = sym->hash) {
		if (!strcmp(name, sym->name))
			return sym;
	}
	if (!install)
		return NULL;

	if ((sym = malloc(sizeof(*sym))) == NULL)
		return NULL;
	len = strlen(name) + 1;
	if ((s = malloc(len)) == NULL) {
		free(sym);
		return NULL;
	}
	sym->name = memcpy(s, name, len);
	sym->type = 'U';
	sym->size = 0;
	sym->value = 0;
	sym->hash = obj->htab[h];
	obj->htab[h] = sym;
	sym->next = obj->head;
	obj->head = sym;

	return sym;
}
