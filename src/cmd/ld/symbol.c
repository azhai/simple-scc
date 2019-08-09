#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <scc/mach.h>
#include <scc/scc.h>

#include "ld.h"

#define NR_SYMBOL 128

static Symbol *symtab[NR_SYMBOL];

Symbol *
lookup(char *name)
{
	unsigned h;
	Symbol *sym;

	h = genhash(name) % NR_SYMBOL;
	for (sym = symtab[h]; sym; sym = sym->hash) {
		if (!strcmp(name, sym->name))
			return sym;
	}

	return NULL;
}

Symbol *
install(char *name)
{
	unsigned h;
	size_t len;
	Symbol *sym;
	char *s;

	h = genhash(name) % NR_SYMBOL;
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
	sym->next = sym->prev = NULL;

	return sym;
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
