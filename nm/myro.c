static char sccsid[] = "@(#) ./nm/myro.c";

#include <ctype.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../inc/scc.h"
#include "../inc/myro.h"
#include "nm.h"

static char *strings;

static int
typeof(struct myrosym *sym)
{
	int t, flags = sym->flags;

	switch (sym->section) {
	case MYRO_TEXT:
		t = 't';
		break;
	case MYRO_DATA:
		t = 'd';
		break;
	case MYRO_BSS:
		t = (flags & MYROSYM_COMMON) ? 'c' : 'b';
		break;
	case MYRO_ABS:
		t = 'a';
		break;
	default:
		t = (flags & MYROSYM_UNDEF) ? 'u' : '?';
		break;
	}
	if (flags & MYROSYM_ABS)
		t = 'a';
	if (flags & MYROSYM_EXTERN)
		t = tolower(t);
	return t;
}

static int
cmp(const void *p1, const void *p2)
{
	const struct myrosym *s1 = p1, *s2 = p2;

	if (vflag)
		return s1->offset - s2->offset;
	else
		return strcmp(strings + s1->name, strings + s2->name);
}

void
nm(char *fname, char *member, FILE *fp)
{
	struct myrohdr hdr;
	struct myrosym *syms = NULL, *sym;
	struct symbol symbol;
	size_t n, i;
	long off;

	strings = NULL;
	if (rdmyrohdr(fp, &hdr) < 0) {
		fprintf(stderr, "nm: %s: incorrect header\n", member);
		return;
	}

	n = hdr.symsize / MYROSYM_SIZ;
	if (n == 0) {
		fprintf(stderr, "nm: %s: no name list\n", member);
		return;
	}
	if (n > SIZE_MAX / sizeof(struct myrosym) ||
	    hdr.symsize / MYROSYM_SIZ > SIZE_MAX ||
	    hdr.strsize > SIZE_MAX) {
		goto offset_overflow;
	}

	syms = xmalloc(n * sizeof(struct myrosym));
	strings = xmalloc(hdr.strsize);
	fread(strings, hdr.strsize, 1, fp);
	if (feof(fp))
		goto free_arrays;
	if ((off = ftell(fp)) < 0)
		return;
	if (off > LONG_MAX - hdr.secsize)
		goto offset_overflow;
	off += hdr.secsize;

	if (fseek(fp, off, SEEK_SET) < 0)
		goto free_arrays;

	for (i = 0; i < n; ++i) {
		if (rdmyrosym(fp, &syms[i]) < 0)
			goto symbol_error;
		if (syms[i].name >= hdr.strsize)
			goto offset_overflow;
	}
	qsort(syms, n, sizeof(*syms), cmp);
	for (i = 0; i < n; ++i) {
		sym = &syms[i];
		symbol.name = strings + sym->name;
		symbol.type = typeof(sym);
		symbol.off = sym->offset;
		symbol.size = sym->len;
		print(fname, member, &symbol);
	}

free_arrays:
	free(syms);
	free(strings);
	return;

symbol_error:
	fprintf(stderr, "nm: %s: error reading symbols\n", fname);
	goto free_arrays;

offset_overflow:
	fprintf(stderr, "nm: %s: overflow in headers of archive\n",
		fname);
	goto free_arrays;
}

int
object(FILE *fp)
{
	char magic[MYROMAGIC_SIZ];
	fpos_t pos;

	fgetpos(fp, &pos);
	fread(magic, sizeof(magic), 1, fp);
	fsetpos(fp, &pos);

	if (!ferror(fp) && !strncmp(magic, MYROMAGIC, MYROMAGIC_SIZ))
		return 1;
	return 0;
}

