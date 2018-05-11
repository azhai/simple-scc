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
	size_t len;

	len = strlen(fname);
	obj = malloc(sizeof(*obj));
	s = malloc(len+1);
	if (!obj || !s)
		outmem();
	obj->fname = memcpy(s, fname, len+1);

	if (!member) {
		obj->member = NULL;
	} else {
		len = strlen(member);
		if ((s = malloc(len+1)) == NULL)
			outmem();
		obj->member = memcpy(s, member, len+1);
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
	return NULL;
}
