static char sccsid[] = "@(#) ./ld/obj.c";

#include <stdlib.h>
#include <string.h>

#include "../inc/scc.h"
#include "ld.h"

Obj *objlst;
static Obj *tail;

Obj *
newobj(char *fname)
{
	Obj *obj;
	char *s;
	size_t len = strlen(fname);

	obj = malloc(sizeof(*obj));
	s = malloc(len+1);
	if (!obj || !s)
		die("ld: out of memory");

	obj->fname = memcpy(s, fname, len);
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
	return NULL;
}
