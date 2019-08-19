#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <scc/mach.h>
#include <scc/scc.h>

#include "ld.h"

#define NR_SYMBOL 128
#define NR_SECTIONS 32

static Symbol *symtab[NR_SYMBOL];
static Section *sectab[NR_SECTIONS];
static Section *seclast;

Section *sechead;

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
	s = malloc(len);
	sym = malloc(sizeof(*sym));
	if (!s || !sym) {
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

Section *
section(char *name)
{
	unsigned h;
	size_t len;
	char *s;
	Section *sec;

	h = genhash(name) % NR_SECTIONS;
	for (sec = sectab[h]; sec; sec = sec->hash) {
		if (!strcmp(name, sec->name))
			return sec;
	}

	len = strlen(name) + 1;
	s = malloc(len);
	sec = malloc(sizeof(*sec));
	if (!s || !sec) {
		error("out of memory");
		exit(EXIT_FAILURE);
	}

	sec->name = memcpy(s, name, len);
	sec->type = '?';
	sec->base = 0;
	sec->size = 0;
	sec->flags = 0;
	sec->hash = sectab[h];
	sectab[h] = sec;

	if (!sechead)
		sechead = sec;
	else
		seclast->next = sec;
	sec->next = NULL;

	return seclast = sec;
}

#ifndef NDEBUG
void
debugsym(void)
{
	Symbol **symp, *sym;

	fputs("Symbols:\n", stderr);
	for (symp = symtab; symp < &symtab[NR_SYMBOL]; symp++) {
		for (sym = *symp; sym; sym = sym->hash)
			fprintf(stderr,
			        "sym: %s (%#llx)\n",
			        sym->name,
			        sym->value);
	}
}

void
debugsec(void)
{
	Section **secp, *sec;

	fputs("Sections:\n", stderr);
	for (secp = sectab; secp < &sectab[NR_SECTIONS]; secp++) {
		for (sec = *secp; sec; sec = sec->hash)
			fprintf(stderr,
			        "sec: %s - %c (%#llx,%#lx)\n",
			        sec->name,
			        sec->type,
			        sec->base,
			        sec->size);
	}

	for (sec = sechead; sec; sec = sec->next) {
		fprintf(stderr,
		        "%s %s",
		        sec->name,
			sec->next ? "->" : "\n");
	}
}
#endif
