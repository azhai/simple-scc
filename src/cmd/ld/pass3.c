#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#include <scc/mach.h>

#include "ld.h"

static void
rebase(Obj *obj)
{
	Symbol *aux;
	Objsym *sym;

	for (sym = obj->syms; sym; sym = sym->next) {
		switch (sym->type) {
		case 'T':
		case 'D':
		case 'B':
			/*
			 * this lookup must succeed, otherwise
			 * we have an error in our code.
			 */
			aux = lookup(sym->name);
			aux->value += obj->secs[sym->sect].base;
		case 't':
		case 'd':
		case 'b':
			sym->value += obj->secs[sym->sect].base;
		case 'U':
		case 'N':
		case '?':
			break;
		default:
			abort();
		}
	}
}

/*
 * rebase all the sections
 */
void
pass3(int argc, char *argv[])
{
	Obj *obj;
	Objlst *lst;
	Objsect *sp;
	unsigned long long *base, text, data, bss;

	/*
	 * TODO: deal with page aligment
	 */
	textbase = text = 0x100;
	database = data = textsiz;
	bssbase = bss = data+datasiz;

	for (lst = objhead; lst; lst = lst->next) {
		obj = lst->obj;
		for (sp = obj->secs; sp; sp = sp->next) {
			switch (sp->type) {
			case 'T':
				base = &text;
				break;
			case 'D':
				base = &data;
				break;
			case 'B':
				base = &bss;
				break;
			default:
				abort();
			}
			sp->base = *base;
			*base += sp->size;
		}
		rebase(obj);
	}
}
