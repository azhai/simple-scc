static char sccsid[] = "@(#) ./ld/obj.c";

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../inc/scc.h"
#include "ld.h"

Obj *objlst;
static Obj *tail;

Obj *
newobj(char *fname, char *member)
{
	Obj *obj;
	char *s, *t;
	size_t l1 = strlen(member), l2 = strlen(fname);

	obj = malloc(sizeof(*obj));
	s = malloc(l1+1);
	t = malloc(l2+1);
	if (!obj || !s || !t)
		outmem();

	obj->fname = memcpy(s, fname, l1);
	obj->member = memcpy(t, member, l2);
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
