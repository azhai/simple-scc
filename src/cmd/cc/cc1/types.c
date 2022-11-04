#include <assert.h>
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>

#include <scc/cstd.h>
#include <scc/scc.h>
#include "cc1.h"

#define NR_TYPE_HASH 16
#define HASH(t) (((t)->op ^ (uintptr_t) (t)->type>>3) & NR_TYPE_HASH-1)

static Type *typetab[NR_TYPE_HASH], *localtypes;

/* FIXME:
 * Compiler can generate warnings here if the ranges of TINT,
 * TUINT and TFLOAT are smaller than any of the constants in this
 * array. Ignore them if you know that the target types are correct
 */
static struct limits limits[][4] = {
	{
		{	/* 0 = unsigned 1 byte */
			.min.i = 0,
			.max.i = 0xff
		},
		{	/* 1 = unsigned 2 bytes */
			.min.i = 0,
			.max.i = 0xffff
		},
		{	/* 2 = unsigned 4 bytes */
			.min.i = 0,
			.max.i = 0xffffffff
		},
		{	/* 3 = unsigned 8 bytes */
			.min.i = 0,
			.max.i = 0xffffffffffffffff
		}
	},
	{
		{	/* 0 = signed 1 byte */
			.min.i = -0x7f-1,
			.max.i = 0x7f
		},
		{	/* 1 = signed 2 byte */
			.min.i = -0x7fff-1,
			.max.i = 0x7fff
		},
		{	/* 2 = signed 4 byte */
			.min.i = -0x7fffffff-1,
			.max.i = 0x7fffffff
		},
		{	/* 3 = signed 8 byte */
			.min.i = -0x7fffffffffffffff-1,
			.max.i = 0x7fffffffffffffff,
		}
	},
	{
		{
			/* 0 = float 4 bytes */
			.min.f = -1,
			.max.f = 2
		},
		{
			/* 1 = float 8 bytes */
			.min.f = -1,
			.max.f = 2,
		},
		{
			/* 2 = float 16 bytes */
			.min.f = -1,
			.max.f = 2,
		}
	}
};

struct limits *
getlimits(Type *tp)
{
	int ntable, ntype;

	switch (tp->op) {
	case ENUM:
	case INT:
		ntable = ((tp->prop & TSIGNED) != 0);
		switch (tp->size) {
		case 1: ntype = 0; break;
		case 2: ntype = 1; break;
		case 4: ntype = 2; break;
		case 8: ntype = 3; break;
		}
		break;
	case FLOAT:
		ntable = 2;
		switch (tp->size) {
		case 4:  ntype = 0; break;
		case 8:  ntype = 1; break;
		case 16: ntype = 2; break;
		}
		break;
	default:
		abort();
	}

	return &limits[ntable][ntype];
}

Type *
ctype(int type, int sign, int size)
{
	switch (type) {
	case CHAR:
		if (size)
			goto invalid_type;
		switch (sign) {
		case 0:
			return chartype;
		case SIGNED:
			return schartype;
		case UNSIGNED:
			return uchartype;
		}
		break;
	case VA_LIST:
		if (size || sign)
			goto invalid_type;
		return va_list_type;
	case VOID:
		if (size || sign)
			goto invalid_type;
		return voidtype;
	case BOOL:
		if (size || sign)
			goto invalid_type;
		return booltype;
	case 0:
	case INT:
		switch (size) {
		case 0:
			return (sign == UNSIGNED) ? uinttype   : inttype;
		case SHORT:
			return (sign == UNSIGNED) ? ushorttype  : shorttype;
		case LONG:
			return (sign == UNSIGNED) ? ulongtype  : longtype;
		case LLONG:
			return (sign == UNSIGNED) ? ullongtype : llongtype;
		}
		break;
	case DOUBLE:
		if (size == LLONG)
			goto invalid_type;
		if (size == LONG)
			size = LLONG;
		else
			size = LONG;
		goto floating;
	case FLOAT:
		if (size == LLONG)
			goto invalid_type;
	floating:
		if (sign)
			goto invalid_type;
		switch (size) {
		case 0:
			return floattype;
		case LONG:
			return doubletype;
		case LLONG:
			return ldoubletype;
		}
		break;
	}

invalid_type:
	error("invalid type specification");
}

void
typesize(Type *tp)
{
	Symbol **sp;
	Type *type;
	unsigned long size, offset;
	int align, a;
	TINT n;

	switch (tp->op) {
	case ARY:
		/* FIXME: Control overflow */
		tp->size = tp->n.elem * tp->type->size;
		tp->align = tp->type->align;
		return;
	case PTR:
		tp->size = pvoidtype->size;
		tp->align = pvoidtype->align;
		return;
	case STRUCT:
	case UNION:
		/* FIXME: Control overflow */
		/*
		 * The alignment of the struct/union is
		 * he alignment of the largest included type.
		 * The size of an union is the size of the largest
		 * field, and the size of a struct is the sum
		 * of the size of every field plus padding bits.
		 */
		offset = align = size = 0;
		n = tp->n.elem;
		for (sp = tp->p.fields; n--; ++sp) {
			type = (*sp)->type;
			a = type->align;
			if (a > align)
				align = a;
			if (tp->op == STRUCT) {
				if (--a != 0)
					offset = (offset + a) & ~a;
				(*sp)->u.i = offset;
				size = offset + type->size;
				offset = size;
			} else {
				(*sp)->u.i = 0;
				if (type->size > size)
					size = type->size;
			}
		}

		tp->align = align;
		/*
		 * We have to add the padding bits to
		 * ensure next struct in an array is well
		 * alignment.
		 */
		if (tp->op == STRUCT && align-- > 1)
			size = size+align & ~align;
		tp->size = size;
		return;
	case ENUM:
		tp->size = inttype->size;
		tp->align = inttype->align;
		return;
	case FTN:
		return;
	default:
		abort();
	}
}

Type *
deftype(Type *tp)
{
	tp->prop |= TDEFINED;
	typesize(tp);
	emit(OTYP, tp);
	return tp;
}

static Type *
newtype(Type *base)
{
	Type *tp;
	size_t siz;

	tp = xmalloc(sizeof(*tp));
	*tp = *base;
	tp->id = newid();

	if (tp->op == FTN) {
		siz = tp->n.elem * sizeof(Type *);
		tp->p.pars = memcpy(xmalloc(siz), tp->p.pars, siz);
	} else if (tp->op == ARY) {
		/* We need alignment for flexible array members */
		tp->align = tp->type->align;
	}

	if (curfun) {
		/* it is a type defined in the body of a function */
		tp->next = localtypes;
		localtypes = tp;
	}
	if (tp->prop & TDEFINED)
		deftype(tp);
	return tp;
}

Type *
mktype(Type *tp, int op, TINT nelem, Type *pars[])
{
	Type **tbl, type;
	Type *bp;

	if (op == PTR && tp == voidtype)
		return pvoidtype;

	type = (Type) {
		.type = tp,
		.op = op,
		.p.pars = pars,
		.n.elem = nelem,
	};

	switch (op) {
	case ARY:
		if (tp == voidtype) {
			errorp("declaration of array of voids type");
			tp = inttype;
		}
		type.letter = L_ARRAY;
		if (nelem != 0)
			type.prop |= TDEFINED;
		break;
	case KRFTN:
		type.prop |= TDEFINED | TK_R;
		type.op = FTN;
		type.letter = L_FUNCTION;
		break;
	case FTN:
		if (nelem > 0 && pars[nelem-1] == ellipsistype)
			type.prop |= TELLIPSIS;
		type.letter = L_FUNCTION;
		type.prop |= TDEFINED;
		break;
	case PTR:
	        type.letter = L_POINTER;
		type.prop |= TDEFINED;
		break;
	case ENUM:
		type.letter = inttype->letter;
		type.prop |= TINTEGER | TARITH;
		type.n.rank = inttype->n.rank;
		goto create_type;
	case STRUCT:
		type.letter = L_STRUCT;
		type.prop |= TAGGREG;
		goto create_type;
	case UNION:
		type.letter = L_UNION;
		type.prop |= TAGGREG;
	create_type:
		return newtype(&type);
	default:
		abort();
	}

	tbl = &typetab[HASH(&type)];
	for (bp = *tbl; bp; bp = bp->h_next) {
		if (eqtype(bp, &type, EQUAL))
			return bp;
	}

	bp = newtype(&type);
	bp->h_next = *tbl;
	*tbl = bp;

	return bp;
}

/*
 * If one type has a parameter type list and the other type is specified by
 * a function declarator that is not part of a function definition and that
 * contains an empty identifier list, the parameter list shall not have an
 * ellipsis terminator and the type of each parameter shall be compatible
 * with the type that results from the application of the default argument
 * promotions.
 */
static int
eqfuns(Type *tp1, Type *tp2, int equiv)
{
	TINT n;
	int f1kr, f2kr;
	Type *krf, *ansi, **pp, *p;

	f1kr = (tp1->prop&TK_R) != 0;
	f2kr = (tp2->prop&TK_R) != 0;

	/* 1: 2 ansi functions */
	if (!f1kr && !f2kr) {
		Type **p1, **p2;
		if (tp1->n.elem != tp2->n.elem)
			return 0;
		p1 = tp1->p.pars, p2 = tp2->p.pars;
		for (n = tp1->n.elem; n > 0; --n) {
			if (!eqtype(*p1++, *p2++, equiv))
				return 0;
		}
		goto check_base;
	}

	/* 2: 2 k&r functions */
	if (f1kr && f2kr)
		goto check_base;

	/* 3: 1 k&r function + 1 ansi function */
	if (!equiv)
		return 0;

	if (f1kr) {
		krf = tp1;
		ansi = tp2;
	} else {
		ansi = tp1;
		krf = tp2;
	}

	for (pp = ansi->p.pars; p = *pp; ++pp) {
		switch (p->op) {
		case ELLIPSIS:
			return 0;
		case INT:
		case ENUM:
			if (p->n.rank < inttype->n.rank)
				return 0;
			break;
		case FLOAT:
			if (p == floattype)
				return 0;
			break;
		}
	}

check_base:
	return eqtype(tp1->type, tp2->type, equiv);
}

int
eqtype(Type *tp1, Type *tp2, int equiv)
{
	TINT n;
	Symbol **s1, **s2;

	if (tp1 == tp2)
		return 1;
	if (!tp1 || !tp2)
		return 0;
	if (tp1->op != tp2->op)
		return 0;

	switch (tp1->op) {
	case UNION:
	case STRUCT:
		if (tp1->letter != tp2->letter)
			return 0;
		if (tp1->tag->name || tp2->tag->name)
			return tp1->tag == tp2->tag;
		if (tp1->n.elem != tp2->n.elem)
			return 0;
		s1 = tp1->p.fields, s2 = tp2->p.fields;
		for (n = tp1->n.elem; n > 0; --n, ++s1, ++s2) {
			if (strcmp((*s1)->name, (*s2)->name))
				return 0;
			if (!eqtype((*s1)->type, (*s2)->type, equiv))
				return 0;
		}
		return 1;
	case FTN:
		return eqfuns(tp1, tp2, equiv);
	case ARY:
		if (equiv && (tp1->n.elem == 0 || tp2->n.elem == 0))
			goto check_base;
		if (tp1->n.elem != tp2->n.elem)
			return 0;
	case PTR:
	check_base:
		return eqtype(tp1->type, tp2->type, equiv);
	case VOID:
	case ENUM:
		return 0;
	case INT:
	case FLOAT:
		return tp1->letter == tp2->letter;
	default:
		abort();
	}
}

void
flushtypes(void)
{
	Type *tp, *next, **h;

	for (tp = localtypes; tp; tp = next) {
		next = tp->next;
		switch (tp->op) {
		default:
			/*
			 * All the local types are linked after
			 * global types, and since we are
			 * unlinking them in the inverse order
			 * we do know that tp is always the head
			 * of the collision list
			 */
			h = &typetab[HASH(tp)];
			assert(*h == tp);
			*h = tp->h_next;
		case STRUCT:
		case UNION:
		case ENUM:
			free(tp);
			break;
		}
	}
	localtypes = NULL;
}
