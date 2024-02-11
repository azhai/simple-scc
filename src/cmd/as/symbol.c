#include <ctype.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <scc/cstd.h>
#include <scc/scc.h>
#include "as.h"

#define HASHSIZ 64
#define NALLOC  10

struct lsymbol {
	Symbol sym;
	struct lsymbol *next;
	struct lsymbol *hash;
};

struct lsection {
	Section s;
	struct lsection *next;
};

Section *cursec;
Section *sabs, *sbss, *sdata, *stext;
Symbol *linesym;
int pass;

static struct lsection *seclist;
static struct lsymbol *hashtbl[HASHSIZ], *symlast, *symlist;

static Symbol *cursym;
static Alloc *tmpalloc;

#ifndef NDEBUG
void
dumpstab(char *msg)
{
	struct lsymbol **bp, *lp;

	fprintf(stderr, "%s\n", msg);
	for (bp = hashtbl; bp < &hashtbl[HASHSIZ]; ++bp) {
		if (*bp == NULL)
			continue;

		fprintf(stderr, "[%d]", (int) (bp - hashtbl));
		for (lp = *bp; lp; lp = lp->hash) {
			fprintf(stderr, " -> %s:%0X:%0X",
			       lp->sym.name,
			       lp->sym.flags,
			       lp->sym.value);
		}
		putc('\n', stderr);
	}
}
#endif

Symbol *
lookup(char *name)
{
	unsigned h;
	Symbol *sym;
	int r, c, symtype;
	struct lsymbol *lp, **list;
	char *t, buf[INTIDENTSIZ+1];

	if (*name == '.' && cursym) {
		if (!cursym)
			error("local label '%s' without global label", name);
		t = cursym->name;
		r = snprintf(buf, sizeof(buf), "%s%s", t, name);
		if (r < 0 || r >= sizeof(buf))
			error("too long local label '%s%s'", t, name);
		name = buf;
	}

	h = genhash(name) & HASHSIZ-1;

	c = toupper(*name);
	list = &hashtbl[h];
	for (lp = *list; lp; lp = lp->hash) {
		sym = &lp->sym;
		t = lp->sym.name;
		if (c == toupper(*t) && !casecmp(t, name))
			return sym;
	}

	lp = xmalloc(sizeof(*lp));
	lp->next = NULL;
	lp->hash = *list;
	*list = lp;

	if (symlast)
		symlast->next = lp;
	symlast = lp;

	if (!symlist)
		symlist = lp;

	sym = &lp->sym;
	sym->name = xstrdup(name);
	sym->flags = 0;
	sym->size = sym->value = 0;
	sym->section = cursec ? cursec->index : -1;

	return sym;
}

Symbol *
deflabel(char *name)
{
	int local = 0;
	Symbol *sym;
	char label[MAXSYM+1];

	if (*name == '.') {
		int r;

		local = 1;
		if (!cursym) {
			error("local label '%s' without global label", name);
			return NULL;
		}
		r = snprintf(label, sizeof(label),
		             "%s%s",
		             cursym->name, name);
		if (r == sizeof(label)) {
			error("local label '%s' in '%s' produces too long symbol",
			      name, cursym->name);
			return NULL;
		}
		name = label;
	}

	sym = lookup(name);
	if (pass == 1 && (sym->flags & FDEF))
		error("redefinition of label '%s'", name);
	if (cursec->flags & SABS)
		sym->flags |= FABS;
	sym->value = cursec->curpc;
	sym->section = cursec->index;

	if (!local)
		cursym = sym;
	return sym;
}

int
toobig(Node *np, int type)
{
	unsigned long long val = np->sym->value;

	switch (type) {
	case AIMM2:
		return val > 3;
	case AIMM3:
		return val > 7;
	case AIMM5:
		return val > 0x1F;
	case AIMM8:
		return val > 0xFF;
	case AIMM16:
		return val > 0xFFFF;
	case AIMM32:
		return val > 0xFFFFFFFF;
	case AIMM64:
		return 1;
	default:
		abort();
	}
}

static void
incpc(int nbytes)
{
	unsigned long long siz;
	TUINT pc, curpc;

	pc = cursec->pc;
	curpc = cursec->curpc;

	cursec->curpc += nbytes;
	cursec->pc += nbytes;

	if (pass == 2)
		return;

	siz =cursec->pc - cursec->base;
	if (siz > cursec->size)
		cursec->size = siz;

	if (pc > cursec->pc ||
	    curpc > cursec->curpc ||
	    cursec->curpc > maxaddr ||
	    cursec->pc > maxaddr) {
		die("as: address overflow in section '%s'");
	}
}

static int
secflags(char *attr)
{
	int c, flags;

	if (!attr)
		return 0;

	for (flags = 0; c = *attr++; ) {
		switch (c) {
		case 'w':
			flags |= SWRITE;
			break;
		case 'r':
			flags |= SREAD;
			break;
		case 'x':
			flags |= SEXEC;
			break;
		case 'f':
			flags |= SFILE;
			break;
		case 'l':
			flags |= SLOAD;
			break;
		case 'a':
			flags |= SABS;
			break;
		}
	}

	return flags;
}

Section *
secindex(int n)
{
	struct lsection *lp;

	for (lp = seclist; lp && lp->s.index != n; lp = lp->next)
		;

	return (lp) ? &lp->s : NULL;
}

static Section *
newsect(Symbol *sym)
{
	Section *sec;
	struct lsection *lp;
	static int index;

	lp = xmalloc(sizeof(*lp));
	lp->next = seclist;
	seclist = lp;

	sec = &lp->s;
	sec->mem = NULL;
	sec->name = xstrdup(sym->name);
	sec->base = sec->size = sec->pc = sec->curpc = 0;
	sec->flags = 0;
	sec->fill = 0;
	sec->align = 0;
	sec->index = index++;

	return sec;
}

Section *
setsec(char *name, char *attr)
{
	Section *sec;
	Symbol *sym;

	cursec = NULL;
	sym = lookup(name);
	if (sym->flags & ~FSECT)
		error("invalid section name '%s'", name);

	sec = secindex(sym->section);
	if (sec == NULL) {
		sec = newsect(sym);
		sym->section = sec->index;
		sym->flags = FSECT;
	}
	sec->flags |= secflags(attr);

	return cursec = sec;
}

void
isecs(void)
{
	sabs = setsec(".abs", "rwx");
	sbss = setsec(".bss", "rwf");
	sdata = setsec(".data", "rw");
	stext = setsec(".text", "rx");
}

void
cleansecs(void)
{
	Section *sec;
	struct lsection *lp;

	for (lp = seclist; lp; lp = lp->next) {
		sec = &lp->s;
		sec->curpc = sec->pc = sec->base;
		if (pass == 1 || sec->flags & SFILE)
			continue;

		if (sec->size > SIZE_MAX)
			die("as: out of memory");
		sec->mem = xmalloc(sec->size);
	}
	cursec = stext;
}

void
emit(char *bytes, int n)
{
	if (cursec->mem) {
		size_t len = cursec->pc - cursec->base;
		memcpy(&cursec->mem[len], bytes, n);
	}
	incpc(n);
}

Symbol *
tmpsym(TUINT val)
{
	Symbol *sym;

	if (!tmpalloc)
		tmpalloc = alloc(sizeof(*sym), NALLOC);
	sym = new(tmpalloc);
	sym->value = val;
	sym->section = -1;
	sym->flags = FABS;

	return sym;
}

void
killtmp(void)
{
	if (!tmpalloc)
		return;
	dealloc(tmpalloc);
	tmpalloc = NULL;
}

int
forallsecs(int (*fn)(Section *, void *), void *arg)
{
	struct lsection *lp;

	for (lp = seclist; lp; lp = lp->next) {
		if ((*fn)(&lp->s, arg) < 0)
			return -1;
	}

	return 0;
}
