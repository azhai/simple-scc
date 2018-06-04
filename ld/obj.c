static char sccsid[] = "@(#) ./ld/obj.c";

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../inc/scc.h"
#include "ld.h"

#define NR_SYM_HASH 64

Obj *objlst;
static Obj *objtail;

static Symbol *secttail;
static Symbol *symtbl[NR_SYM_HASH];

Section *sectlst;

Obj *
add(Obj *obj)
{
	obj->next = NULL;

	if (!objlst) {
		objtail = objlst = obj;
	} else {
		objtail->next = obj;
		objtail = obj;
	}
}

void
delobj(Obj *obj)
{
	free(obj->strtbl);
	free(obj->scnhdr);
	free(obj->filhdr);
	free(obj->fname);
	free(obj->member);
	free(obj);
}

Obj *
newobj(char *fname, char *member, FILE *fp)
{
	Obj *obj;
	char *s, *t;
	size_t len;

	len = strlen(fname);
	obj = malloc(sizeof(*obj));
	s = malloc(len) + 1;
	if (!obj || !s)
		outmem();
	memset(obj, 0, sizeof(*obj));
	obj->fname = memcpy(s, fname, len);

	if (!member) {
		obj->member = NULL;
	} else {
		len = strlen(member) + 1;
		if ((s = malloc(len)) == NULL)
			outmem();
		obj->member = memcpy(s, member, len);
	}

	obj->fp = fp;
	if ((obj->offset = ftell(fp)) == EOF) {
		fprintf(stderr, "ld: %s: %s\n", fname, strerror(errno));
		exit(1);
	}

	return obj;
}

Section *
slookup(char *name)
{
	char *s;
	Section *prev, *sp;
	size_t len = strlen(name);

	for (prev = sp = sectlst; sp; prev = sp, sp = sp->next) {
		if (!memcmp(sp->name, name, len))
			return sp;
	}

	sp = malloc(sizeof(*sp));
	s = malloc(len);
	if (!sp || !s)
		outmem();
	sp->name = s;
	sp->base = 0;
	sp->size = 0;
	sp->next = NULL;

	if (!prev)
		sectlst = sp;
	else
		prev->next = sp;

	return sp;
}

static unsigned
hash(char *s)
{
	unsigned h, c;

	for (h = 0; c = *s; ++s)
		h = h*33 ^ c;
	return h & NR_SYM_HASH-1;
}

Symbol *
lookup(char *name, int install)
{
	unsigned h;
	char *s;
	size_t len;
	Symbol *sym;

	h = hash(name);
	for (sym = symtbl[h]; sym; sym = sym->hash) {
		s = sym->name;
		if (*name == *s && !strcmp(name, s))
			return sym;
	}

	if (!install)
		return NULL;

	len = strlen(name) + 1;
	sym = malloc(sizeof(*sym));
	s = malloc(len);
	if (!sym || !s)
		outmem();
	memset(sym, 0, sizeof(*sym));
	memcpy(s, name, len);

	sym->hash = symtbl[h];
	symtbl[h] = sym;
	sym->name = s;

	return sym;
}
