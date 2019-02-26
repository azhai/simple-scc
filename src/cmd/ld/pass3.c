#include <stdio.h>
#include <stdlib.h>

#include <scc/mach.h>

#include "ld.h"

/*
 * relocate the sections
 */
void
pass3(int argc, char *argv[])
{
	Obj *obj;
	Objlst *lst;
	Objsect *sp;
	unsigned long long text, data, bss;

	textbase = text = 0;
	database = data = textsiz+3 & ~3;
	bssbase = bss = data+datasiz+3 & ~3;

	for (lst = objhead; lst; lst = lst->next) {
		for (sp = lst->obj->secs; sp; sp = sp->next) {
			switch (sp->type) {
			case 'T':
			case 'D':
			case 'B':
			default:
				abort();
			}
		}
	}
}
