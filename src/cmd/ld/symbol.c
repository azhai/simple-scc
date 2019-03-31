#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <scc/mach.h>
#include <scc/scc.h>

#include "ld.h"

#define NR_SYMBOL 128

static Symbol *symtab[NR_SYMBOL];

static Symbol refhead = {
	.next = &refhead,
	.prev = &refhead,
};

Symbol *
lookup(char *name, int install)
{
	size_t len;
	char *s;
	unsigned h;
	Symbol *sym;

	h = genhash(name) % NR_SYMBOL;

	for (sym = symtab[h]; sym; sym = sym->hash) {
		if (!strcmp(name, sym->name))
			return sym;
	}

	if (!install)
		return NULL;

	len = strlen(name) + 1;
	sym = malloc(sizeof(*sym));
	s = malloc(len);
	if (!len || !s) {
		error("out of memory");
		exit(EXIT_FAILURE);
	}

	sym->obj = NULL;
	sym->name = memcpy(s, name, len);
	sym->hash = symtab[h];
	symtab[h] = sym;
	sym->value = 0;
	sym->size = 0;

	refhead.next->prev = sym;
	sym->next = refhead.next;
	refhead.next = sym;
	sym->prev = &refhead;

	return sym;
}

Symbol *
define(Objsym *osym, Obj *obj)
{
	Symbol *sym = lookup(osym->name, INSTALL);

	if (sym->def && sym->def->type != 'C') {
		error("%s: symbol redefined", osym->name);
		return NULL;
	}

	sym->obj = obj;
	sym->def = osym;
	sym->size = osym->size;
	sym->value = osym->value;

	sym->next->prev = sym->prev;
	sym->prev->next = sym->next;
	sym->next = sym->prev = NULL;

	return sym;
}

int
newsym(Objsym *osym, Obj *obj)
{
	Symbol *sym;

	switch (osym->type) {
	case 'U':
		lookup(osym->name, INSTALL);
	case '?':
	case 'N':
		break;
	case 'C':
		sym = lookup(osym->name, NOINSTALL);
		if (!sym || !sym->def) {
			define(osym, obj);
			break;
		}
		if (sym->def->type != 'C')
			break;
		if (sym->size < osym->size)
			sym->size = osym->size;
		break;
	default:
		if (isupper(osym->type))
			define(osym, obj);
		break;
	}

	return 1;
}

int
moreundef(void)
{

	return refhead.next != &refhead;
}

void
listundef(void)
{
	Symbol *sym, *p;

	p = &refhead;
	for (sym = p->next; sym != p; sym = sym->next) {
		fprintf(stderr,
		        "ld: symbol '%s' not defined\n",
		        sym->name);
	}
}

int
defasym(Obj *obj)
{
	Symbol *sym, *p;

	p = &refhead;
	for (sym = p->next; sym != p; sym = sym->next) {
		if (objlookup(obj, sym->name, 0))
			return 1;
	}

	return 0;
}

#ifndef NDEBUG
int
debugsym(void)
{
	Symbol **symp, *sym;

	for (symp = symtab; symp < &symtab[NR_SYMBOL]; symp++) {
		for (sym = *symp; sym; sym = sym->hash)
			fprintf(stderr,
			        "sym: %s (%#x)\n",
			        sym->name,
			        sym->value);
	}
}
#endif
