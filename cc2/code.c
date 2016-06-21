/* See LICENSE file for copyright and license details. */
#include <stdlib.h>

#include "../inc/cc.h"
#include "arch.h"
#include "cc2.h"

Inst *pc, *prog;

static void
nextpc(void)
{
        Inst *new;

        new = xcalloc(1, sizeof(*new)); /* TODO: create an arena */

        if (!pc) {
                prog = new;
        } else {
                new->next = pc->next;
                pc->next = new;
        }

	/* SNONE being 0, calloc initialized {from1,from2,to}.kind for us */
        new->prev = pc;
        pc = new;
}

static void
addr(Node *np, Addr *addr)
{
	Symbol *sym;

	switch (np->op) {
	case OREG:
		/* TODO:
		 * At this moment this op is used also for register variables
		 */
		addr->kind = SREG;
		addr->u.reg = np->u.reg;
		break;
	case OCONST:
		addr->kind = SCONST;
		/* TODO: Add support for more type of constants */
		addr->u.i = np->u.i;
		break;
	case OTMP:
	case OLABEL:
	case OAUTO:
	case OMEM:
		sym = np->u.sym;
		addr->kind = sym->kind;
		addr->u.sym = sym;
		break;
	default:
		abort();
	}
}

Symbol *
newlabel(void)
{
	Symbol *sym = getsym(TMPSYM);

	sym->kind = SLABEL;
	return sym;
}

Node *
label2node(Symbol *sym)
{
	Node *np;

	np = newnode(OLABEL);
	np->u.sym = sym;

	return np;
}

void
setlabel(Symbol *sym)
{
	if (!sym)
		return;
	code(0, NULL, NULL, NULL);
	pc->label = sym;
}

void
code(int op, Node *to, Node *from1, Node *from2)
{
	nextpc();
	if (from1)
		addr(from1, &pc->from1);
	if (from2)
		addr(from2, &pc->from2);
	if (to)
		addr(to, &pc->to);
	pc->op = op;
}

void
delcode(void)
{
        Inst *prev = pc->prev, *next = pc->next;

        free(pc);
        if (!prev) {
                pc = next;
                prog = NULL;
        } else {
                pc = prev;
                prev->next = next;
                if (next)
                        next->prev = prev;
        }
}
