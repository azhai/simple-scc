#include <assert.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <scc/mach.h>
#include <scc/scc.h>

#include "ld.h"

#define NR_SECTION 32

/*
 * struct sectab has a Section as first field because
 * the code is going to cast from the sections to the tab.
 */
struct sectab {
	Section sec;
	FILE *tmpfp;
	struct sectab *hash;
	struct sectab *next, *prev;
};

static struct sectab *sectab[NR_SECTION];
static struct sectab secs = {.next = &secs, .prev = &secs};

static Section *
linksec(struct sectab *lst, Section *sec)
{
	struct sectab *sp = (struct sectab *) sec;

	sp->next = lst;
	sp->prev = lst->prev;
	lst->prev->next = sp;
	lst->prev = sp;

	return sec;
}

Section *
lookupsec(char *name)
{
	unsigned h;
	size_t len;
	char *s;
	Section *sec;
	struct sectab *sp;

	h = genhash(name) % NR_SECTION;
	for (sp = sectab[h]; sp; sp = sp->hash) {
		if (!strcmp(name, sp->sec.name))
			return &sp->sec;
	}

	len = strlen(name) + 1;
	s = malloc(len);
	sp = malloc(sizeof(*sp));
	if (!s || !sp) {
		error(strerror(errno));
		exit(EXIT_FAILURE);
	}

	sec = &sp->sec;
	sec->name = memcpy(s, name, len);
	sec->type = 'U';
	sec->base = 0;
	sec->size = 0;
	sec->align = 0;
	sec->index = 0;
	sec->flags = 0;
	sp->tmpfp;
	sp->hash = sectab[h];
	sectab[h] = sp;

	sp->next = &secs;
	sp->prev = secs.prev;
	secs.prev->next = sp;
	secs.prev = sp;

	return sec;
}

void
merge(Segment *seg)
{
	struct sectab *sp;
	Section *sec, **p;
	int n = 0;

	for (sp = secs.next; sp != &secs; sp = sp->next) {
		sec = &sp->sec;
		if (sec->type != seg->type)
			continue;
		p = realloc(seg->sections, (n+1) * sizeof(*p));
		if (!p) {
			error("out of memory");
			exit(EXIT_FAILURE);
		}
		p[n++] = sec;
		seg->sections = p;
		seg->size += sec->size;
	}

	seg->nsec = n;
}

void
copy(Obj *obj, Section *osec, Section *sec)
{
	struct sectab *sp = (struct sectab *) sec;

	if (sec->size > ULLONG_MAX - osec->size) {
		error("%s: section too long", sec->name);
		return;
	}

	if (!sp->tmpfp && (sp->tmpfp = tmpfile()) == NULL) {
		error(strerror(errno));
		exit(EXIT_FAILURE);
	}

/*
	if (mapsec(obj, osec->index, sp->tmpfp) < 0) {
		error(strerror(errno));
		return;
	}
*/

	sec->size += osec->size;
}

void
grow(Section *sec, int nbytes)
{
	struct sectab *sp = (struct sectab *) sec;

	if (sec->size > ULLONG_MAX - nbytes) {
		error("%s: section too long", sec->name);
		return;
	}

	if (!sp->tmpfp && (sp->tmpfp = tmpfile()) == NULL) {
		error(strerror(errno));
		exit(EXIT_FAILURE);
	}

	while (nbytes-- > 0)
		putc(0, sp->tmpfp);

	sec->size += nbytes;
}

#ifndef NDEBUG
void
debugsec(void)
{
	struct sectab **spp, *sp;
	Section *sec;

	fputs("Sections:\n", stderr);
	for (spp = sectab; spp < &sectab[NR_SECTION]; spp++) {
		for (sp = *spp; sp; sp = sp->hash) {
			sec = &sp->sec;
			fprintf(stderr,
			        "sec: %s - %c (%#llx,%#lx)\n",
			        sec->name,
			        sec->type,
			        sec->base,
			        sec->size);
		}
	}
}
#endif
