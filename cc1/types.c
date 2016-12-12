/* See LICENSE file for copyright and license details. */
static char sccsid[] = "@(#) ./cc1/types.c";
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <cstd.h>
#include "../inc/cc.h"
#include "cc1.h"

#define NR_TYPE_HASH 16

/* FIXME:
 * Compiler can generate warnings here if the ranges of TINT,
 * TUINT and TFLOAT are smaller than any of the constants in this
 * array. Ignore them if you know that the target types are correct
 */
static struct limits limits[][4] = {
	{
		{	/* 0 = unsigned 1 byte */
			.min.i = 0,
			.max.i = 255
		},
		{	/* 1 = unsigned 2 bytes */
			.min.i = 0,
			.max.i = 65535u
		},
		{	/* 2 = unsigned 4 bytes */
			.min.i = 0,
			.max.i = 4294967295u
		},
		{	/* 3 = unsigned 8 bytes */
			.min.i = 0,
			.max.i = 18446744073709551615u
		}
	},
	{
		{	/* 0 = signed 1 byte */
			.min.i = -127,
			.max.i = 127
		},
		{	/* 1 = signed 2 byte */
			.min.i = -32767,
			.max.i = 32767
		},
		{	/* 2 = signed 4 byte */
			.min.i = -2147483647L,
			.max.i = 2147483647L
		},
		{	/* 3 = signed 8 byte */
			.min.i = -9223372036854775807LL,
			.max.i = 9223372036854775807LL,
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

static Type typetab[NR_TYPE_HASH];

void
itypes()
{
	Type *tp;

	for (tp = typetab; tp < &typetab[NR_TYPE_HASH]; ++tp) {
		tp->h_next = tp;
		tp->h_prev = tp;
	}
}

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
ctype(unsigned type, unsigned sign, unsigned size)
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
	case VOID:
		if (size || sign)
			goto invalid_type;
		return voidtype;
	case BOOL:
		if (size || sign)
			goto invalid_type;
		return booltype;
	case 0:
		/* fallthrough */
	case INT:
		switch (size) {
		case 0:
			return (sign == UNSIGNED) ? uinttype   : inttype;
		case SHORT:
			return (sign == UNSIGNED) ? ushortype  : shortype;
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
			(*sp)->u.i = offset;
			type = (*sp)->type;
			a = type->align;
			if (a > align)
				align = a;
			if (tp->op == STRUCT) {
				if (--a != 0)
					size = (size + a) & ~a;
				size += type->size;
				offset = size;
			} else {
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
			size += size+align & ~align;
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
mktype(Type *tp, int op, TINT nelem, Type *pars[])
{
	Type *tbl, *h_next, type;
	unsigned t;
	Type *bp;
	int c, k_r = 0;

	if (op == PTR && tp == voidtype)
		return pvoidtype;

	if (op == KRFTN) {
		k_r = 1;
		op = FTN;
	}
	switch (op) {
	case PTR:     c = L_POINTER;       break;
	case ARY:     c = L_ARRAY;         break;
	case FTN:     c = L_FUNCTION;      break;
	case ENUM:    c = inttype->letter; break;
	case STRUCT:  c = L_STRUCT;        break;
	case UNION:   c = L_UNION;         break;
	}

	memset(&type, 0, sizeof(type));
	type.type = tp;
	type.op = op;
	type.prop = k_r ? TK_R : 0;
	type.letter = c;
	type.p.pars = pars;
	type.n.elem = nelem;
	type.ns = 0;

	switch (op) {
	case ARY:
		if (nelem == 0)
			break;
		/* PASSTROUGH */
	case FTN:
	case PTR:
		type.prop |= TDEFINED;
		break;
	case ENUM:
		type.prop |= TPRINTED | TINTEGER | TARITH;
		type.n.rank = inttype->n.rank;
		break;
	case STRUCT:
	case UNION:
		type.prop |= TAGGREG;
		break;
	default:
		abort();
	}

	t = (op ^ (uintptr_t) tp>>3) & NR_TYPE_HASH-1;
	tbl = &typetab[t];
	for (bp = tbl; bp->h_next != tbl; bp = bp->h_next) {
		if (eqtype(bp, &type, 0) && op != STRUCT && op != UNION) {
			/*
			 * pars was allocated by the caller
			 * but the type already exists, so
			 * we have to deallocte it
			 */
			free(pars);
			return bp;
		}
	}

	if (type.prop & TDEFINED)
		typesize(&type);
	bp = xmalloc(sizeof(*bp));
	*bp = type;
	bp->id = newid();

	/* insert the new type in the circular double list */
	h_next = tbl->h_next;
	bp->h_next = h_next;
	bp->h_prev = h_next->h_prev;
	h_next->h_prev = bp;
	tbl->h_next = bp;

	return bp;
}

int
eqtype(Type *tp1, Type *tp2, int equiv)
{
	TINT n;
	Type **p1, **p2;

	if (tp1 == tp2)
		return 1;
	if (!tp1 || !tp2)
		return 0;
	if (tp1->op != tp2->op)
		return 0;
	switch (tp1->op) {
	case UNION:
	case STRUCT:
	case FTN:
		if (tp1->n.elem != tp2->n.elem)
			return 0;
		p1 = tp1->p.pars, p2 = tp2->p.pars;
		for (n = tp1->n.elem; n > 0; --n) {
			if (!eqtype(*p1++, *p2++, equiv))
				return 0;
		}
		goto check_base;
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
