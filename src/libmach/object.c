static char sccsid[] = "@(#) ./libmach/object.c";

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <scc/mach.h>

#include "libmach.h"

static struct format *fmts[] = {
	[COFF32] = &objcoff32,
	[NFORMATS] = NULL,
};

int
objtest(FILE *fp, char **name)
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

	for (bp = fmts; bp < &fmts[NFORMATS]; ++bp) {
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

int
objopen(FILE *fp, int type, Obj *obj)
{
	struct format *op;

	obj->type = type;
	obj->symtbl = NULL;
	obj->data = NULL;
	obj->nsym = obj->cursym = 0;
	op = fmts[FORMAT(type)];
	if ((*op->open)(fp, type, obj) < 0)
		return -1;
	return 0;
}

static int
addsym(Obj *obj, Symbol *sym)
{
	Symbol *p, *new;
	char *s;
	size_t len, siz = obj->nsym * sizeof(*sym);

	if (siz > SIZE_MAX - sizeof(*sym))
		return -1;
	siz += sizeof(*sym);
	if ((p = realloc(obj->symtbl, siz)) == NULL)
		return -1;
	obj->symtbl = p;

	new = &p[obj->nsym];
	new->type = sym->type;
	len = strlen(sym->name) + 1;
	if ((new->name = malloc(len)) == NULL)
		return -1;
	memcpy(new->name, sym->name, len);
	new->size = sym->size;
	new->value = sym->value;
	obj->nsym++;

	return 0;
}

int
objread(FILE *fp, Obj *obj, int (*filter)(Symbol *))
{
	int r;
	Symbol sym, *p;
	struct format *op;

	op = fmts[FORMAT(obj->type)];
	while ((r = (*op->read)(obj, &sym)) > 0) {
		if (filter && (*filter)(&sym))
			continue;
		addsym(obj, &sym);
	}

	return r;
}

void
objclose(Obj *obj)
{
	struct format *op;

	op = fmts[FORMAT(obj->type)];
	(*op->close)(obj);
}
