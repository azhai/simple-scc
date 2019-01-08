#include <ctype.h>
#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <scc/arg.h>
#include <scc/mach.h>

#define NR_NAMES 32

typedef struct name Name;

struct name {
	char *name;
	int type;
	long offset;
	Name *hash, *next;
};

static int status;
static char *filename, *membname;
static Name *htab[NR_NAMES], *head;
static long offset;
char *argv0;

static void
error(char *fmt, ...)
{
	va_list va;

	va_start(va, fmt);
	fprintf(stderr, "ranlib: %s: ", filename);
	if (membname)
		fprintf(stderr, "%s: ", membname);
	vfprintf(stderr, fmt, va);
	putc('\n', stderr);
	va_end(va);

	status = EXIT_FAILURE;
}

Name *
lookup(char *name)
{
	unsigned h;
	Name *np;
	char *s;
	size_t len;

	h = 0;
	for (s = name; *s; s++)
		h += *s;
	h %= NR_NAMES;

	for (np = htab[h]; np; np = np->next) {
		if (!strcmp(np->name, name))
			return np;
	}

	len = strlen(name) + 1;
	np = malloc(sizeof(*np));
	s = malloc(len);
	if (!np || !s) {
		free(s);
		free(np);
		return NULL;
	}

	memcpy(np->name, s, len);
	np->type = 'U';
	np->offset = -1;
	np->hash = htab[h];
	htab[h] = np;
	np->next = head;
	head = np;

	return np;
}

static int
newsymbol(Symbol *sym, void *data)
{
	Name *np;

	if (!isupper(sym->type))
		return 1;

	if ((np = lookup(sym->name)) == NULL) {
		error("out of memory");
		return 0;
	}

	switch (np->type) {
	case 'C':
		if (sym->type == 'C')
			break;
	case 'U':
		np->type = sym->type;
		np->offset = offset;
		break;
	default:
		if (sym->type != 'C') {
			error("multiple definitions of '%s'", sym->name);
			return 0;
		}
	}

	return 1;
}

static int
newmember(FILE *fp, char *name, void *data)
{

	int t, ret = 0;
	Obj *obj;

	membname = name;
	offset = ftell(fp);

	if (offset == EOF) {
		error(strerror(errno));
		return 0;
	}

	if ((t = objtype(fp, NULL)) == -1)
		return 1;

	if ((obj = objnew(t)) == NULL) {
		error("out of memory");
		return 0;
	}

	if (objread(obj, fp) < 0) {
		error("file corrupted");
		goto error;
	}

	if (!objtraverse(obj, newsymbol, NULL)) {
		error("traversing object file");
		goto error;
	}

	ret = 1;

error:
	objdel(obj);
	return ret;
}

static void
freenames(void)
{
	Name **npp, *next, *np;

	for (npp = htab; npp < &htab[NR_NAMES]; npp++)
		*npp = NULL;

	for (np = head; np; np = next) {
		next = np->next;
		free(np->name);
		free(np);
	}

	head = NULL;
}

static int
readsyms(char *fname)
{
	FILE *fp;

	if ((fp = fopen(fname, "rb")) == NULL) {
		error(strerror(errno));
		goto error;
	}

	if (!archive(fp)) {
		error("file format not recognized");
		goto error;
	}

	if (!artraverse(fp, newmember, NULL)) {
		error("while traversing archive");
		goto error;
	}

	if (fclose(fp)) {
		error(strerror(errno));
		return 0;
	}
	return 1;

error:
	fclose(fp);
	return 0;
}

static int
writeidx(char *fname)
{
	int r;
	FILE *fp;
	Name *np;

	if ((fp = fopen(fname, "wb")) == NULL) {
		error("index file: %s", strerror(errno));
		return 0;
	}

	for (np = head; np; np = np->next) {
		/* TODO: write out */
	}
	fflush(fp);
	r = ferror(fp);
	fclose(fp);

	if (!r)
		return 1;

	error("index file: %s", strerror(errno));
	return 0;
}

static int
insertidx(char *archive, char *idx)
{
	return 0;
}

static void
ranlib(char *fname)
{
	static char symdef[] = "__.SYMDEF";

	filename = fname;
	freenames();

	if (!readsyms(fname))
		return;

	if (!writeidx(symdef))
		return;

	if (!insertidx(fname, symdef))
		remove(symdef);

	return;
}

static void
usage(void)
{
	fputs("usage: ranlib [file...]\n", stderr);
	exit(EXIT_FAILURE);
}

int
main(int argc, char *argv[])
{
	ARGBEGIN {
	default:
		usage();
	} ARGEND

	if (argc == 0) {
		ranlib("a.out");
	} else {
		for (; *argv; ++argv)
			ranlib(*argv);
	}

	return status;
}
