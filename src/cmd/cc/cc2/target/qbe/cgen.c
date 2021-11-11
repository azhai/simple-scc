#include <assert.h>
#include <stdlib.h>

#include <scc/cstd.h>
#include <scc/scc.h>

#include "arch.h"
#include "../../cc2.h"

static char opasmw[] = {
	[OADD] = ASADDW,
	[OSUB] = ASSUBW,
	[OMUL] = ASMULW,
	[OMOD] = ASMODW,
	[ODIV] = ASDIVW,
	[OSHL] = ASSHLW,
	[OSHR] = ASSHRW,
	[OLT] = ASLTW,
	[OGT] = ASGTW,
	[OLE] = ASLEW,
	[OGE] = ASGEW,
	[OEQ] = ASEQW,
	[ONE] = ASNEW,
	[OBAND] = ASBANDW,
	[OBOR] = ASBORW,
	[OBXOR] = ASBXORW,
};

static char opasml[] = {
	[OADD] = ASADDL,
	[OSUB] = ASSUBL,
	[OMUL] = ASMULL,
	[OMOD] = ASMODL,
	[ODIV] = ASDIVL,
	[OSHL] = ASSHLL,
	[OSHR] = ASSHRL,
	[OLT] = ASLTL,
	[OGT] = ASGTL,
	[OLE] = ASLEL,
	[OGE] = ASGEL,
	[OEQ] = ASEQL,
	[ONE] = ASNEL,
	[OBAND] = ASBANDL,
	[OBOR] = ASBORL,
	[OBXOR] = ASBXORL,
};

static char opasms[] = {
	[OADD] = ASADDS,
	[OSUB] = ASSUBS,
	[OMUL] = ASMULS,
	[ODIV] = ASDIVS,
	[OLT] = ASLTS,
	[OGT] = ASGTS,
	[OLE] = ASLES,
	[OGE] = ASGES,
	[OEQ] = ASEQS,
	[ONE] = ASNES,
};
static char opasmd[] = {
	[OADD] = ASADDD,
	[OSUB] = ASSUBD,
	[OMUL] = ASMULD,
	[ODIV] = ASDIVD,
	[OLT] = ASLTD,
	[OGT] = ASGTD,
	[OLE] = ASLED,
	[OGE] = ASGED,
	[OEQ] = ASEQD,
	[ONE] = ASNED,
};

extern Type int32type, uint32type, ptrtype;

/*
 * This is strongly influenced by
 * http://plan9.bell-labs.com/sys/doc/compiler.ps (/sys/doc/compiler.ps)
 * calculate addresability as follows
 *     AUTO => 11          value+fp
 *     REG => 11           reg
 *     STATIC => 11        (value)
 *     CONST => 11         $value
 * These values of addressability are not used in the code generation.
 * They are only used to calculate the Sethi-Ullman numbers. Since
 * QBE is AMD64 targered we could do a better job there, and try to
 * detect some of the complex addressing modes of these processors.
 */
static Node *
complex(Node *np)
{
	Node *lp = np->left, *rp = np->right;

	if (np->address > 10)
		return np;
	if (lp)
		np->complex = lp->complex;
	if (rp) {
		int d = np->complex - rp->complex;

		if (d == 0)
			++np->complex;
		else if (d < 0)
			np->complex = rp->complex;
	}
	if (np->complex == 0)
		++np->complex;

	return np;
}

Node *
sethi(Node *np)
{
	Node *lp, *rp;

	if (!np)
		return np;

	np->complex = 0;
	np->address = 0;
	lp = np->left;
	rp = np->right;

	switch (np->op) {
	case OAUTO:
	case OREG:
	case OMEM:
	case OCONST:
		np->address = 11;
		break;
	case OASSIG:
		if (lp->op == OCAST) {
			Node *tmp = node(OCAST);
			tmp->type = lp->left->type;
			tmp->left = rp;
			tmp->right = NULL;
			rp = tmp;
			tmp = lp;
			lp = lp->left;
			delnode(tmp);
		}
		goto binary;
	case OCPL:
		assert(np->type.flags & INTF);
		np->op = OBXOR;
		rp = constnode(NULL, ~(TUINT) 0, &np->type);
		goto binary;
	case OSNEG:
		np->op = OSUB;
		rp = lp;
		lp = constnode(NULL, 0, &np->type);
		if ((np->type.flags & INTF) == 0)
			lp->u.f = 0.0;
	default:
	binary:
		lp = sethi(lp);
		rp = sethi(rp);
		break;
	}
	np->left = lp;
	np->right = rp;

	return complex(np);
}

static Node *
load(Type *tp, Node *np)
{
	int op;
	Node *new;
	int flags = tp->flags;

	if (flags & (AGGRF|FUNF|ARRF))
		return np;

	switch (tp->size) {
	case 1:
		op = ASLDSB;
		break;
	case 2:
		op = ASLDSH;
		break;
	case 4:
		op = (flags & FLOATF) ? ASLDS : ASLDSW;
		break;
	case 8:
		op = (flags & FLOATF) ? ASLDD : ASLDL;
		break;
	default:
		abort();
	}
	/*
	 * unsigned version of operations are always +1 the
	 * signed version
	 */
	if ((flags & (INTF|SIGNF)) == INTF && tp->size < 8)
		++op;

	new = tmpnode(NULL, tp);
	code(op, new, np, NULL);

	return new;
}

static Node *rhs(Node *np);

static Node *
cast(Type *td, Node *np)
{
	Type *ts;
	Node *tmp;
	int op, d_isint, s_isint;

	ts = &np->type;
	d_isint = (td->flags & INTF) != 0;
	s_isint = (ts->flags & INTF) != 0;

	if (d_isint && s_isint) {
		if (td->size <= ts->size)
			return np;

		assert(td->size == 4 || td->size == 8);
		switch (ts->size) {
		case 1:
			op = (td->size == 4) ? ASEXTBW : ASEXTBL;
			break;
		case 2:
			op = (td->size == 4) ? ASEXTHW : ASEXTHL;
			break;
		case 4:
			op = ASEXTWL;
			break;
		default:
			abort();
		}
		/*
		 * unsigned version of operations are always +1 the
		 * signed version
		 */
		op += (ts->flags & SIGNF) == 0;
	} else if (d_isint) {
		/* conversion from float to int */
		switch (ts->size) {
		case 4:
			op = (td->size == 8) ? ASSTOL : ASSTOW;
			break;
		case 8:
			op = (td->size == 8) ? ASDTOL : ASDTOW;
			break;
		default:
			abort();
		}
		/* TODO: Add signess */
	} else if (s_isint) {
		/* conversion from int to float */
		switch (ts->size) {
		case 1:
		case 2:
			ts = (ts->flags&SIGNF) ? &int32type : &uint32type;
			np = cast(ts, np);
		case 4:
			op = (td->size == 8) ? ASSWTOD : ASSWTOS;
			break;
		case 8:
			op = (td->size == 8) ? ASSLTOD : ASSLTOS;
			break;
		default:
			abort();
		}
		/* TODO: Add signess */
	} else {
		/* conversion from float to float */
		op = (td->size == 4) ? ASEXTS : ASTRUNCD;
	}

	tmp = tmpnode(NULL, td);
	code(op, tmp, np, NULL);

	return tmp;
}

static Node *
call(Node *np, Node *fun)
{
	int n, op;
	Type *tp;
	Node **q, *tmp, *p, *pars[NR_FUNPARAM];

	for (n = 0, p = np->right; p; p = p->right)
		pars[n++] = rhs(p->left);

	tp = &np->type;
	tmp = tmpnode(NULL, tp);
	code(ASCALL, tmp, fun, NULL);

	for (q = pars; q < &pars[n]; ++q) {
		op = (q == &pars[n-1]) ? ASPARE : ASPAR;
		code(op, NULL, *q, tmpnode(NULL, &(*q)->type));
	}
	code((np->op == OCALL) ? ASCALLE : ASCALLEX, NULL, NULL, NULL);

	return tmp;
}

static Node *
copy(Type *tp, Node *to, Node *from)
{
	int op;

	switch (tp->size) {
	case 0:
		return from;
	case 1:
		op = ASCOPYB;
		break;
	case 2:
		op = ASCOPYH;
		break;
	case 4:
		op = (tp->flags & FLOATF) ? ASCOPYS : ASCOPYW;
		break;
	case 8:
		op = (tp->flags & FLOATF) ? ASCOPYD : ASCOPYL;
		break;
	default:
		/* TODO: Need to handle the general case */
		abort();
	}
	code(op, to, from, NULL);
	return from;
}

static Node *
field(Node *np, int islhs)
{
	Node *tmp, *addr;
	TUINT offset = np->right->u.sym->u.off;

	addr = rhs(np->left);
	tmp = node(OADD);
	tmp->type = ptrtype;
	tmp->left = addr;
	tmp->right = constnode(NULL, offset, &ptrtype);
	addr = rhs(tmp);

	if (!islhs)
		addr = load(&np->type, addr);
	return addr;
}

static Node *
lhs(Node *np)
{
	switch (np->op) {
	case OREG:
	case OMEM:
	case OAUTO:
		return np;
	case OPTR:
		return rhs(np->left);
	case OFIELD:
		return field(np, 1);
	default:
		abort();
	}
}

static void
bool(Node *np, Symbol *true, Symbol *false)
{
	Node *l = np->left, *r = np->right;
	Node ret, ifyes, ifno;
	Symbol *label;

	switch (np->op) {
	case ONEG:
		bool(l, false, true);
		break;
	case OAND:
		label = newlabel();
		bool(l, label, false);
		setlabel(label);
		bool(r, true, false);
		break;
	case OOR:
		label = newlabel();
		bool(l, true, label);
		setlabel(label);
		bool(r, true, false);
		break;
	default:
		label2node(&ifyes, true);
		label2node(&ifno, false);
		code(ASBRANCH, rhs(np), &ifyes, &ifno);
		break;
	}
}

static Node *
ternary(Node *np)
{
	Node ifyes, ifno, phi, *colon, *tmp;

	tmp = tmpnode(NULL, &np->type);
	label2node(&ifyes, NULL);
	label2node(&ifno, NULL);
	label2node(&phi, NULL);

	colon = np->right;
	code(ASBRANCH, rhs(np->left), &ifyes, &ifno);

	setlabel(ifyes.u.sym);
	copy(&tmp->type, tmp, rhs(colon->left));
	code(ASJMP, NULL, &phi, NULL);

	setlabel(ifno.u.sym);
	copy(&tmp->type, tmp, rhs(colon->right));
	setlabel(phi.u.sym);

	return tmp;
}

static Node *
function(void)
{
	Node aux;
	Symbol *p;

	/* allocate stack space for parameters */
	for (p = locals; p && (p->type.flags & PARF) != 0; p = p->next)
		code(ASALLOC, label2node(&aux, p), NULL, NULL);

	/* allocate stack space for local variables) */
	for ( ; p && p->id != TMPSYM; p = p->next) {
		if (p->kind != SAUTO)
			continue;
		code(ASALLOC, label2node(&aux, p), NULL, NULL);
	}
	/* store formal parameters in parameters */
	for (p = locals; p; p = p->next) {
		if ((p->type.flags & PARF) == 0)
			break;
		code(ASFORM, label2node(&aux, p), NULL, NULL);
	}
	return NULL;
}

static void
swtch_if(Node *idx)
{
	Node aux1, aux2, *np;
	Symbol *deflabel = NULL;

	for (;;) {
		np = delstmt();
		setlabel(np->label);

		switch (np->op) {
		case OESWITCH:
			if (!deflabel)
				deflabel = np->u.sym;
			aux1.op = OJMP;
			aux1.label = NULL;
			aux1.u.sym = deflabel;
			cgen(&aux1);
			return;
		case OCASE:
			aux1 = *np;
			aux1.op = OBRANCH;
			aux1.label = NULL;
			aux1.left = &aux2;

			aux2.op = OEQ;
			aux2.type = idx->type;
			aux2.left = np->left;
			aux2.right = idx;

			cgen(&aux1);
			break;
		case ODEFAULT:
			deflabel = np->u.sym;
			break;
		default:
			abort();
		}
	}
}

static int
assignop(Type *tp)
{
	int flags = tp->flags;

	if (flags & (AGGRF|FUNF|ARRF))
		return ASSTM;

	switch (tp->size) {
	case 1:
		return ASSTB;
	case 2:
		return ASSTH;
	case 4:
		return (tp->flags & FLOATF) ? ASSTS : ASSTW;
	case 8:
		return (tp->flags & FLOATF) ? ASSTD : ASSTL;
	default:
		abort();
	}
}

static Node *
assign(Node *np)
{
	Node *tmp, aux;
	Node *l = np->left, *r = np->right;
	int op;

	switch (np->u.subop) {
	case OINC:
		op = OADD;
		goto post_oper;
	case ODEC:
		op = OSUB;
	post_oper:
		tmp = rhs(l);
		aux.op = op;
		aux.left = tmp;
		aux.right = r;
		aux.type = np->type;
		r = complex(&aux);
		break;
	default:
		/* assign abbreviation */
		aux.type = np->type;
		aux.op = np->u.subop;
		aux.right = np->right;
		aux.left = np->left;
		r = rhs(complex(&aux));
	case 0:
		op = 0;
		break;
	}

	if (l->complex >= r->complex) {
		l = lhs(l);
		r = rhs(r);
	} else {
		r = rhs(r);
		l = lhs(l);
	}

	code(assignop(&np->type), l, r, NULL);

	return (op == 0) ? r : tmp;
}

static Node *
rhs(Node *np)
{
	Node *tmp, aux1, aux2;
	Node *phi, *l = np->left, *r = np->right;
	Type *tp;
	int off, op;
	char *tbl;
	Symbol *true, *false;

	tp = &np->type;

	switch (np->op) {
	case OBFUN:
		return function();
	case ONOP:
	case OBLOOP:
	case OELOOP:
	case OEFUN:
		return NULL;
	case OTMP:
	case OCONST:
		return np;
	case OMEM:
	case OREG:
	case OAUTO:
		return load(tp, np);
	case ONEG:
	case OAND:
	case OOR:
		true = newlabel();
		false = newlabel();
		phi = label2node(&aux1, NULL);
		tmp = tmpnode(NULL, &int32type);

		bool(np, true, false);

		setlabel(true);
		code(ASCOPYW, tmp, constnode(&aux2, 1, &int32type), NULL);
		code(ASJMP, NULL, phi, NULL);

		setlabel(false);
		code(ASCOPYW, tmp, constnode(&aux2, 0, &int32type), NULL);

		setlabel(phi->u.sym);
		return tmp;
        case OMOD:
        case OSHR:
		assert(tp->flags & INTF);
        case ODIV:
        case OLT:
        case OGT:
        case OLE:
        case OGE:
                /*
                 * unsigned version of operations are always +1 the
                 * signed version
                 */
                off = (tp->flags & SIGNF) == 0;
                goto binary;
        case OSHL:
        case OBAND:
        case OBOR:
        case OBXOR:
		assert(tp->flags & INTF);
        case OADD:
        case OSUB:
        case OMUL:
        case OEQ:
        case ONE:
                off = 0;
        binary:
		if (l->complex >= r->complex) {
			l = rhs(l);
			r = rhs(r);
		} else {
			r = rhs(r);
			l = rhs(l);
		}

                switch (tp->size) {
                case 4:
                        tbl = (tp->flags & FLOATF) ? opasms : opasmw;
                        break;
                case 8:
                        tbl = (tp->flags & FLOATF) ? opasmd : opasml;
                        break;
                default:
                        abort();
                }
		op = tbl[np->op] + off;
		tmp = tmpnode(NULL, tp);
		code(op, tmp, l, r);
		return tmp;
	case OCALL:
	case OCALLE:
		if (l->op == OPTR)
			l = rhs(l);
		return call(np, l);
	case OCAST:
		return cast(tp, rhs(l));
	case OASSIG:
		return assign(np);
	case OASK:
		return ternary(np);
	case OCOMMA:
		rhs(l);
		return rhs(r);
	case OPTR:
		return load(tp, rhs(l));
	case OADDR:
		l = lhs(l);
		l->type = *tp;
		return l;
	case OFIELD:
		return field(np, 0);
	case OBUILTIN:
		switch (np->u.subop) {
		case BVA_START:
			l = rhs(l);
			code(ASVSTAR, NULL, l, NULL);
			return NULL;
		case BVA_END:
			return NULL;
		case BVA_ARG:
			l = rhs(l);
			tmp = tmpnode(NULL, tp);
			code(ASVARG, tmp, l, NULL);
			return tmp;
		case BVA_COPY:
			/* TODO */
		default:
			abort();
		}
	default:
		abort();
	}
	abort();
}

Node *
cgen(Node *np)
{
	Node aux, *p, *next;

	setlabel(np->label);
	switch (np->op) {
	case OJMP:
		label2node(&aux, np->u.sym);
		code(ASJMP, NULL, &aux, NULL);
		break;
	case OBRANCH:
		next = np->next;
		if (!next->label)
			next->label = newlabel();
		bool(np->left, np->u.sym, next->label);
		break;
	case ORET:
		p = (np->left) ? rhs(np->left) : NULL;
		code(ASRET, NULL, p, NULL);
		break;
	case OBSWITCH:
		p = rhs(np->left);
		swtch_if(p);
		break;
	default:
		rhs(np);
		break;
	}
	return NULL;
}
