#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <scc/scc.h>

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
	case OMREG:
		addr->kind = SREG;
		addr->u.reg = np->u.reg;
		break;
	case OCONST:
		addr->kind = SCONST;
		/* TODO: Add support for more type of constants */
		addr->u.i = np->u.i;
		break;
	case OREG:
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

void
pprint(char *s)
{
	int c;
	char *t;

	putchar('"');
	while ((c = *s++) != '\0') {
		switch (c) {
		case '\n':
			t = "\\n";
			goto print_str;
		case '\v':
			t = "\\v";
			goto print_str;
		case '\b':
			t = "\\b";
			goto print_str;
		case '\t':
			t = "\\t";
			goto print_str;
		case '\a':
			t = "\\a";
			goto print_str;
		case '\f':
			t = "\\f";
			goto print_str;
		case '\r':
			t = "\\r";
			goto print_str;
		case '"':
			t = "\\\"";
			goto print_str;
		case '\'':
			t = "\\'";
			goto print_str;
		case '\?':
			t = "\\\?";
			goto print_str;
		case '\\':
			putchar('\\');
		default:
			if (!isprint(c))
				printf("\\x%x", c);
			else
				putchar(c);
			break;
		print_str:
			fputs(t, stdout);
			break;
		}
	}
	putchar('"');
}

Symbol *
newlabel(void)
{
	Symbol *sym = getsym(TMPSYM);

	sym->kind = SLABEL;
	return sym;
}

Node *
label2node(Node *np, Symbol *sym)
{
	if(!sym)
		sym = newlabel();
	if (!np)
		np = node(OLABEL);
	np->op = OLABEL;
	np->u.sym = sym;

	return np;
}

Node *
tmpnode(Type *tp)
{
	char flags;
	Symbol *sym;
	Node *np;

	np = node(OTMP);
	sym = getsym(TMPSYM);
	sym->type = np->type = *tp;
	flags = tp->flags & ~(PARF|INITF);
	sym->type.flags = np->type.flags = flags;
	sym->kind = STMP;
	np->left = np->right = NULL;
	np->u.sym = sym;
	np->op = OTMP;
	return np;
}

Node *
constnode(Node *np, TUINT n, Type *tp)
{
	if (!np)
		np = node(OCONST);
	np->op = OCONST;
	np->left = NULL;
	np->right = NULL;
	np->type = *tp;
	np->u.i = n;
	return np;
}

void
setlabel(Symbol *sym)
{
	if (!sym)
		return;
	code(0, NULL, NULL, NULL);
	pc->label = sym;
	sym->u.inst = pc;
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
