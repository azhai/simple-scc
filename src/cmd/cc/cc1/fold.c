#include <assert.h>
#include <stdlib.h>

#include <scc/scc.h>
#include "cc1.h"


TUINT
ones(int nbytes)
{
	return (nbytes == 8) ? -1 : ~(-1ull << nbytes * 8);
}

static int
addi(TINT l, TINT r, Type *tp)
{
	struct limits *lim = getlimits(tp);
	TINT max = lim->max.i, min = lim->min.i;

	if (l < 0 && r < 0 && l >= min - r ||
	    l == 0 ||
	    r == 0 ||
	    l < 0 && r > 0 ||
	    l > 0 && r < 0 ||
	    l > 0 && r > 0 && l <= max - r) {
		return 1;
	}
	warn("overflow in constant expression");
	return 0;
}

static int
addf(TFLOAT l, TFLOAT r, Type *tp)
{
	struct limits *lim = getlimits(tp);
	TFLOAT max = lim->max.f, min = lim->min.f;

	if (l < 0 && r < 0 && l >= min - r ||
	    l == 0 ||
	    r == 0 ||
	    l < 0 && r > 0 ||
	    l > 0 && r < 0 ||
	    l > 0 && r > 0 && l <= max - r) {
		return 1;
	}
	warn("overflow in constant expression");
	return 0;
}

static int
subi(TINT l, TINT r, Type *tp)
{
	return addi(l, -r, tp);
}

static int
subf(TFLOAT l, TFLOAT r, Type *tp)
{
	return addf(l, -r, tp);
}

static int
muli(TINT l, TINT r, Type *tp)
{
	struct limits *lim = getlimits(tp);
	TINT max = lim->max.i, min = lim->min.i;

	if (l > -1 && l <= 1 ||
	    r > -1 && r <= 1 ||
	    l < 0 && r < 0 && -l <= max/-r ||
	    l < 0 && r > 0 &&  l >= min/r  ||
	    l > 0 && r < 0 &&  r >= min/l  ||
	    l > 0 && r > 0 &&  l <= max/r) {
			return 1;
	}
	warn("overflow in constant expression");
	return 0;
}

static int
mulf(TFLOAT l, TFLOAT r, Type *tp)
{
	struct limits *lim = getlimits(tp);
	TFLOAT max = lim->max.f, min = lim->min.f;

	if (l > -1 && l <= 1 ||
	    r > -1 && r <= 1 ||
	    l < 0 && r < 0 && -l <= max/-r ||
	    l < 0 && r > 0 &&  l >= min/r  ||
	    l > 0 && r < 0 &&  r >= min/l  ||
	    l > 0 && r > 0 &&  l <= max/r) {
			return 1;
	}
	warn("overflow in constant expression");
	return 0;
}

static int
divi(TINT l, TINT r,  Type *tp)
{
	struct limits *lim = getlimits(tp);

	if (r == 0 || l == -lim->min.i && r == -1) {
		warn("overflow in constant expression");
		return 0;
	}
	return 1;
}

static int
divf(TFLOAT l, TFLOAT r,  Type *tp)
{
	struct limits *lim = getlimits(tp);

	if (l < 0) l = -l;
	if (r < 0) r = -r;

	if (r == 0.0 || r < 1.0 && l > lim->max.f * r) {
		warn("overflow in constant expression");
		return 0;
	}
	return 1;
}

static int
lshi(TINT l, TINT r, Type *tp)
{
	if (r < 0 || r >= tp->size * 8) {
		warn("shifting %d bits is undefined", r);
		return 0;
	}
	return muli(l, 1 << r, tp);
}

static int
rshi(TINT l, TINT r, Type *tp)
{
	if (r < 0 || r >= tp->size * 8) {
		warn("shifting %d bits is undefined", r);
		return 0;
	}
	return 1;
}

static int
foldint(int op, Symbol *res, TINT l, TINT r)
{
	TINT i;
	Type *tp = res->type;
	int (*validate)(TINT, TINT, Type *tp);

	switch (op) {
	case OADD: validate = addi; break;
	case OSUB: validate = subi; break;
	case OMUL: validate = muli; break;
	case ODIV: validate = divi; break;
	case OSHL: validate = lshi; break;
	case OSHR: validate = rshi; break;
	case OMOD: validate = divi; break;
	default:   validate = NULL; break;
	}

	if (validate && !(*validate)(l, r, tp))
		return 0;

	switch (op) {
	case OADD:  i = l + r;  break;
	case OSUB:  i = l - r;  break;
	case OMUL:  i = l * r;  break;
	case ODIV:  i = l / r;  break;
	case OMOD:  i = l % r;  break;
	case OSHL:  i = l << r; break;
	case OSHR:  i = l >> r; break;
	case OBAND: i = l & r;  break;
	case OBXOR: i = l ^ r;  break;
	case OBOR:  i = l | r;  break;
	case OAND:  i = l && r; break;
	case OOR:   i = l || r; break;
	case OLT:   i = l < r;  break;
	case OGT:   i = l > r;  break;
	case OGE:   i = l >= r; break;
	case OLE:   i = l <= r; break;
	case OEQ:   i = l == r; break;
	case ONE:   i = l != r; break;
	case ONEG:  i = !l;     break;
	case OSNEG: i = -l;     break;
	case OCPL:  i = ~l;     break;
	default:    return 0;
	}
	res->u.i = i;

	DBG("FOLD i l=%lld %d r=%lld = %lld", l, op, r, i);
	return 1;
}

static int
folduint(int op, Symbol *res, TUINT l, TUINT r)
{
	TINT i;
	TUINT u;

	switch (op) {
	case OADD:  u = l + r;  break;
	case OSUB:  u = l - r;  break;
	case OMUL:  u = l * r;  break;
	case ODIV:  u = l / r;  break;
	case OMOD:  u = l % r;  break;
	case OSHL:  u = l << r; break;
	case OSHR:  u = l >> r; break;
	case OBAND: u = l & r;  break;
	case OBXOR: u = l ^ r;  break;
	case OBOR:  u = l | r;  break;
	case ONEG:  u = !l;     break;
	case OSNEG: u = -l;     break;
	case OCPL:  u = ~l;     break;
	case OAND:  i = l && r; goto sign;
	case OOR:   i = l || r; goto sign;
	case OLT:   i = l < r;  goto sign;
	case OGT:   i = l > r;  goto sign;
	case OGE:   i = l >= r; goto sign;
	case OLE:   i = l <= r; goto sign;
	case OEQ:   i = l == r; goto sign;
	case ONE:   i = l != r; goto sign;
	default:    return 0;
	}
	res->u.u = u & ones(res->type->size);

	DBG("FOLD ui l=%llu %d r=%llu = %llu", l, op, r, u);
	return 1;

sign:
	res->u.i = i;

	DBG("FOLD sui %llu %d %llu = %llu", l, op, r, i);
	return 1;
}

static int
foldfloat(int op, Symbol *res, TFLOAT l, TFLOAT r)
{
	TFLOAT f;
	TINT i;
	int (*validate)(TFLOAT, TFLOAT, Type *tp);

	switch (op) {
	case OADD: validate = addf; break;
	case OSUB: validate = subf; break;
	case OMUL: validate = mulf; break;
	case ODIV: validate = divf; break;
	default:   validate = NULL; break;
	}

	if (validate && !(*validate)(l, r, res->type))
		return 0;

	switch (op) {
	case OADD: f = l + r;  break;
	case OSUB: f = l - r;  break;
	case OMUL: f = l * r;  break;
	case ODIV: f = l / r;  break;
	case OLT:  i = l < r;  goto comparison;
	case OGT:  i = l > r;  goto comparison;
	case OGE:  i = l >= r; goto comparison;
	case OLE:  i = l <= r; goto comparison;
	case OEQ:  i = l == r; goto comparison;
	case ONE:  i = l != r; goto comparison;
	default:   return 0;
	}
	res->u.f = f;

	DBG("FOLD f l=%lf %d r=%lf = %lf", l, op, r, f);
	return 1;

comparison:
	res->u.i = i;

	DBG("FOLD if l=%lf %d r=%lf = %lld", l, op, r, i);
	return 1;
}

static Node *
foldconst(int type, int op, Type *tp, Symbol *ls, Symbol *rs)
{
	Symbol *sym, aux;
	TINT i;
	TUINT u;
	TFLOAT f;

	aux.type = tp;
	switch (type) {
	case INT:
		i = (rs) ? rs->u.i : 0;
		if (!foldint(op, &aux, ls->u.i, i))
			return NULL;
		break;
	case PTR:
	case UNSIGNED:
		u = (rs) ? rs->u.u : 0u;
		if (!folduint(op, &aux, ls->u.u, u))
			return NULL;
		break;
	case FLOAT:
		f = (rs) ? rs->u.f : 0.0;
		if (!foldfloat(op, &aux, ls->u.f, f))
			return NULL;
		break;
	default:
		abort();
	}
	sym = newsym(NS_IDEN, NULL);
	sym->flags |= SCONSTANT;
	sym->type = tp;
	sym->u = aux.u;
	return constnode(sym);
}

static Node *
foldcast(Node *np, Node *l)
{
	TUINT negmask, mask, u;
	Type *newtp = np->type, *oldtp = l->type;
	Symbol aux, *sym, *osym = l->sym;

	if ((l->flags & NCONST) == 0 || !osym)
		return np;

	switch (newtp->op) {
	case PTR:
	case INT:
	case ENUM:
		switch (oldtp->op) {
		case PTR:
		case INT:
		case ENUM:
			u = (oldtp->prop & TSIGNED) ? osym->u.i : osym->u.u;
			break;
		case FLOAT:
			oldtp = newtp;
			u = osym->u.f;
			break;
		default:
			return  np;
		}
		mask = ones(newtp->size);
		if (newtp->prop & TSIGNED) {
			negmask = ~mask;
			if (u & (negmask >> 1) & mask)
				u |= negmask;
			aux.u.i = u;
		} else {
			aux.u.u = u & mask;
		}
		break;
	case FLOAT:
		/* FIXME: The cast can be from another float type */
		aux.u.f = (oldtp->prop & TSIGNED) ? osym->u.i : osym->u.u;
		break;
	default:
		return np;
	}

	DBG("FOLD cast %c->%c", oldtp->letter, newtp->letter);
	freetree(np);
	sym = newsym(NS_IDEN, NULL);
	sym->flags |= SCONSTANT;
	sym->type = newtp;
	sym->u = aux.u;
	return constnode(sym);
}

static Node *
foldunary(Node *np)
{
	Node *l = np->left;
	Node *aux, *aux2;;
	int op = l->op;

	switch (np->op) {
	case ONEG:
		if (l->op == ONEG)
			break;
		return np;
	case OADD:
		DBG("FOLD unary delete %d", np->op);
		np->left = NULL;
		freetree(np);
		return l;
	case OCAST:
		return foldcast(np, l);
	case OSNEG:
	case OCPL:
		if (op != np->op)
			return np;
		break;
	case OPTR:
		if (op != OADDR || np->type != l->left->type)
			return np;
		break;
	case OADDR:
		/* &(*s).f -> s + offsetof(typeof(*s), f) */
		if (op == OFIELD && l->left->op == OPTR) {
			DBG("FOLD collapse '&(*s).f' %d", np->op);
			aux = offsetnode(l->right->sym, np->type);
			aux = node(OADD, np->type, l->left->left, aux);

			if (aux->left->flags & NCONST)
				aux->flags |= NCONST;
			l->left->left = NULL;
			freetree(np);
			return aux;
		}

		/* &s.f -> &s + offsetof(typeof(s), f) */
		if (op == OFIELD) {
			DBG("FOLD collapse '&s.f' %d", np->op);
			aux = offsetnode(l->right->sym, np->type);
			aux2 = node(OADDR, np->type, l->left, NULL);
			aux = node(OADD, np->type, aux2, aux);

			if (l->flags & NCONST)
				aux->flags |= NCONST;
			l->left = NULL;
			freetree(np);
			return aux;
		}

		if (op != OPTR)
			return np;
		break;
	default:
		return np;
	}
	DBG("FOLD unary cancel %d", np->op);
	aux = l->left;
	l->left = NULL;
	freetree(np);
	return aux;
}

static Node *
fold(Node *np)
{
	Symbol *rs, *ls;
	Type *optype;
	int type;
	int op = np->op;
	Node *p, *lp = np->left, *rp = np->right;
	Type *tp = np->type;

	if (!lp && !rp)
		return np;

	if ((op == ODIV || op == OMOD) && cmpnode(rp, 0)) {
		warn("division by 0");
		return np;
	}
	/*
	 * Return if any of the children is no constant,
	 * or it is a constant generated when
	 * the address of a static variable is taken
	 * (when we don't know the physical address so
	 * we cannot fold it)
	 */
	if (!rp) {
		rs = NULL;
	} else {
		if (!(rp->flags & NCONST) || !rp->sym)
			return np;
		rs = rp->sym;
	}

	if ((lp->flags & NCONST) == 0 || !lp->sym)
		return np;
	optype = lp->type;
	ls = lp->sym;

	type = optype->op;
	switch (type) {
	case ENUM:
	case INT:
		if (!(optype->prop & TSIGNED))
			type = UNSIGNED;
	case PTR:
	case FLOAT:
		if ((p = foldconst(type, op, tp, ls, rs)) == NULL) {
			np->flags &= ~NCONST;
			return np;
		}
		freetree(np);
		return p;
	default:
		return np;
	}
}

static void
commutative(Node *np)
{
	int op = np->op;
	Node *l = np->left, *r = np->right;

	if (r == NULL || r->flags&NCONST || !(l->flags&NCONST))
		return;

	switch (op) {
	case OLT:
	case OGT:
	case OGE:
	case OLE:
		DBG("FOLD neg commutative %d", np->op);
		np->op = negop(op);
	case OEQ:
	case ONE:
	case OADD:
	case OMUL:
	case OBAND:
	case OBXOR:
	case OBOR:
		DBG("FOLD commutative %d", np->op);
		np->left = r;
		np->right = l;
		break;
	}
}

static Node *
identity(Node *np)
{
	int iszeror, isoner;
	int iszerol, isonel;
	Node *lp = np->left, *rp = np->right;

	if (!rp)
		return np;

	iszeror = cmpnode(rp, 0);
	isoner = cmpnode(rp, 1),
	iszerol = cmpnode(lp, 0);
	isonel = cmpnode(lp, 1);

	switch (np->op) {
	case OOR:
		/*
		 * 1 || i => 1    (free right)
		 * i || 0 => i    (free right)
		 * 0 || i => i    (free left)
		 * i || 1 => i,1  (comma)
		 */
		if (isonel | iszeror)
			goto free_right;
		if (iszerol)
			goto free_left;
		if (isoner)
			goto change_to_comma;
		return np;
	case OAND:
		/*
		 * 0 && i => 0    (free right)
		 * i && 1 => i    (free right)
		 * 1 && i => i    (free left)
		 * i && 0 => i,0  (comma)
		 */
		if (iszerol | isoner)
			goto free_right;
		if (isonel)
			goto free_left;
		if (iszeror)
			goto change_to_comma;
		return np;
	case OSHL:
	case OSHR:
		/*
		 * i >> 0 => i    (free right)
		 * i << 0 => i    (free right)
		 * 0 >> i => 0    (free right)
		 * 0 << i => 0    (free right)
		 */
		if (iszeror | iszerol)
			goto free_right;
		return np;
	case OBXOR:
	case OADD:
	case OBOR:
	case OSUB:
		/*
		 * i + 0  => i
		 * i - 0  => i
		 * i | 0  => i
		 * i ^ 0  => i
		 */
		if (iszeror)
			goto free_right;
		return np;
	case OMUL:
		/*
		 * i * 0  => i,0 (comma)
		 * i * 1  => i   (free right)
		 */
		if (iszeror)
			goto change_to_comma;
		if (isoner)
			goto free_right;
		return np;
	case ODIV:
		/* i / 1  => i */
		if (isoner)
			goto free_right;
		return np;
	case OBAND:
		/* i & ~0 => i */
		if (cmpnode(rp, -1))
			goto free_right;
		return np;
	case OMOD:
		/* i % 1  => i,1 */
		if (isoner)
			goto change_to_comma;
	default:
		return np;
	}

free_right:
	DBG("FOLD identity %d", np->op);
	np->left = NULL;
	freetree(np);
	return lp;

free_left:
	DBG("FOLD identity %d", np->op);
	np->right = NULL;
	freetree(np);
	return rp;

change_to_comma:
	DBG("FOLD identity %d", np->op);
	np->op = OCOMMA;
	return np;
}

static Node *
foldternary(Node *np)
{
	Node *cond = np->left, *body = np->right;

	if ((cond->flags & NCONST) == 0)
		return np;
	if (cmpnode(cond, 0)) {
		np = body->right;
		freetree(body->left);
	} else {
		np = body->left;
		freetree(body->right);
	}

	DBG("FOLD ternary");
	body->left = NULL;
	body->right = NULL;
	freetree(cond);
	free(body);
	return np;
}

static Node *xsimplify(Node *);

static void
reduce(Node *np)
{
	Node *lp = np->left, *rp = np->right;
	Node *aux, *aux2;
	int op = np->op;

	switch (op) {
	case OMOD:
		/* i % 2^n => i & n-1 */
		if (power2node(rp, NULL)) {
			np->op = OBAND;
			rp->sym->u.u--;
			break;
		}
		return;
	default:
		return;
	}

	DBG("FOLD reduce %d->%d", op, np->op);
}

static void
associative(Node *np)
{
	Node *l = np->left, *r = np->right;

	switch (np->op) {
	case OADD:
	case OMUL:
	case OBAND:
	case OBXOR:
	case OBOR:
		if (np->op != l->op
		|| l->right->op != OSYM
		|| !(l->right->sym->flags&SCONSTANT)) {
			return;
		}

		DBG("FOLD associative %d", np->op);
		np->left = l->left;
		l->left = r;
		np->right = fold(l);
		break;
	}
}

/* TODO: fold OCOMMA */
static Node *
xxsimplify(Node *np)
{
	int op;

	np->left = xsimplify(np->left);
	np->right = xsimplify(np->right);

repeat:
	switch (op = np->op) {
	case OASK:
		np = foldternary(np);
		break;
	case OCALL:
	case OPAR:
	case OSYM:
	case OASSIGN:
	case OA_MUL:
	case OA_DIV:
	case OA_MOD:
	case OA_ADD:
	case OA_SUB:
	case OA_SHL:
	case OA_SHR:
	case OA_AND:
	case OA_XOR:
	case OA_OR:
		break;
	case OSNEG:
	case OCPL:
	case OADDR:
	case OPTR:
	case INC:
	case DEC:
	case OCAST:
	case ONEG:
		assert(!np->right);
		np = foldunary(np);
		np = fold(np);
		break;
	default:
		commutative(np);
		associative(np);
		np = fold(np);
		np = identity(np);
		reduce(np);
		break;
	}

	if (op != np->op)
		goto repeat;
	return np;
}

static Node *
xsimplify(Node *np)
{
	if (!np)
		return NULL;

	if (enadebug)
		prtree("simplify before", np);
	np = xxsimplify(np);
	if (enadebug)
		prtree("simplify after", np);

	return np;
}

Node *
simplify(Node *np)
{
	DBG("SIMPLIFY");
	return xsimplify(np);
	DBG("SIMPLIFY DONE");
}
