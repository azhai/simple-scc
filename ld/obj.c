static char sccsid[] = "@(#) ./ld/obj.c";

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../inc/scc.h"
#include "ld.h"

#define NR_SYM_HASH 64

Obj *objlst;
static Obj *tail;

static Symbol *symtbl[NR_SYM_HASH];

Obj *
newobj(char *fname, char *member)
{
	Obj *obj;
	char *s, *t;
	size_t len;

	len = strlen(fname);
	obj = malloc(sizeof(*obj));
	s = malloc(len) + 1;
	if (!obj || !s)
		outmem();
	obj->fname = memcpy(s, fname, len);

	if (!member) {
		obj->member = NULL;
	} else {
		len = strlen(member) + 1;
		if ((s = malloc(len)) == NULL)
			outmem();
		obj->member = memcpy(s, member, len);
	}
	obj->next = NULL;

	if (!objlst)
		tail = objlst = obj;
	else
		tail->next = obj;

	return obj;
}

Symbol *
lookup(char *name)
{
	unsigned h, c;
	char *s;
	size_t len;
	Symbol *sym;

	for (h = 0; c = *name; ++s)
		h = h*33 ^ c;
	h &= NR_SYM_HASH-1;

	for (sym = symtbl[h]; sym; sym = sym->hash) {
		s = sym->name;
		if (*name == *s && !strcmp(name, s))
			return sym;
	}

	len = strlen(name) + 1;
	sym = malloc(sizeof(*sym));
	s = malloc(len);
	if (!sym || !s)
		outmem();
	sym->hash = symtbl[h];
	symtbl[h] = sym;
	sym->name = s;
	memset(sym, 0, sizeof(*sym));
	memcpy(sym->name, name, len);

	return sym;
}
