static char sccsid[] = "@(#) ./libmach/object.c";

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <scc/mach.h>

#include "libmach.h"

int
objtype(FILE *fp, char **name)
{
	int n, i;
	int (*fn)(unsigned char *, char **);
	struct format **bp, *op;
	fpos_t pos;
	unsigned char buf[NBYTES];

	fgetpos(fp, &pos);
	n = fread(buf, NBYTES, 1, fp);
	fsetpos(fp, &pos);

	if (n != 1 || ferror(fp))
		return -1;

	for (bp = objfmt; bp < &objfmt[NFORMATS]; ++bp) {
		op = *bp;
		if (!op || !op->probe)
			continue;
		n = (*op->probe)(buf, name);
		if (n == -1)
			continue;
		return n;
	}

	return -1;
}

Symbol *
objlookup(Obj *obj, char *name)
{
	unsigned h;
	size_t len;
	char *s;
	Symbol *sym;

	h = 0;
	for (s = name; *s; s++)
		h += *s;
	h %= NR_SYMHASH;

	for (sym = obj->htab[h]; sym; sym = sym->hash) {
		if (!strcmp(name, sym->name))
			return sym;
	}

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

int
objwrite(Obj *obj, FILE *fp)
{
	int fmt;
	struct format *op;

	fmt = FORMAT(obj->type);
	if (fmt >= NFORMATS)
		return -1;
	op = objfmt[fmt];
	if ((*op->write)(obj, fp) < 0)
		return -1;
	return 0;
}

int
objread(Obj *obj, FILE *fp)
{
	int fmt;
	struct format *op;

	fmt = FORMAT(obj->type);
	if (fmt >= NFORMATS)
		return -1;
	op = objfmt[fmt];
	if ((*op->read)(obj, fp) < 0)
		return -1;
	return 0;
}

Obj *
objnew(int type)
{
	Obj *obj;
	int fmt;
	struct format *op;

	fmt = FORMAT(type);
	if (fmt >= NFORMATS)
		return NULL;

	if ((obj = malloc(sizeof(*obj))) == NULL)
		return NULL;

	obj->type = type;
	obj->head = NULL;
	memset(obj->htab, 0, sizeof(obj->htab));

	op = objfmt[fmt];
	if ((*op->new)(obj) < 0) {
		free(obj);
		return NULL;
	}

	return obj;
}

int
objtraverse(Obj *obj, int (*fn)(Symbol *, void *), void *data)
{
	Symbol *sym;

	for (sym = obj->head; sym; sym = sym->next) {
		 if (!(*fn)(sym, data))
			return 0;
	}
	return 1;
}

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

void
objreset(Obj *obj)
{
	int fmt;
	struct format *op;

	fmt = FORMAT(obj->type);
	if (fmt < NFORMATS) {
		op = objfmt[fmt];
		(*op->del)(obj);
	}
	delsyms(obj);
}

void
objdel(Obj *obj)
{
	objreset(obj);
	free(obj);
}

void
objstrip(Obj *obj)
{
	int fmt;
	struct format *op;

	fmt = FORMAT(obj->type);
	if (fmt >= NFORMATS)
		return -1;
	op = objfmt[fmt];
	(*op->strip)(obj);
	delsyms(obj);
}
