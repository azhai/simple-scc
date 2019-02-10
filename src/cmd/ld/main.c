static char sccsid[] = "@(#) ./ld/main.c";

#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <scc/mach.h>
#include <scc/scc.h>
#include <scc/ar.h>
#include <scc/syslibs.h>

#define NR_SYMBOL 128

typedef struct objlst Objlst;
typedef struct symbol Symbol;

enum {
	OUTLIB,
	INLIB,
};

struct objlst {
	Obj *obj;
	struct objlst *next;
};

struct symbol {
	char *name;
	Obj *obj;
	Objsym *sym;
	struct symbol *next, *prev;
	struct symbol *hash;
};

char *output = "a.out", *entry = "start", *datasiz;

static char *filename, *membname;
static Objlst *objhead, *objlast;
static Symbol *symtab[NR_SYMBOL];
static Symbol refhead = {
	.next = &refhead,
	.prev = &refhead,
};

int sflag;		/* discard all the symbols */
int xflag;		/* discard local symbols */
int Xflag;		/* discard locals starting with 'L' */
int rflag;		/* preserve relocation bits */
int dflag;		/* define common even with rflag */
int gflag;              /* preserve debug symbols */

static int status;

static char *
errstr(void)
{
	return strerror(errno);
}

static void
error(char *fmt, ...)
{
	va_list va;

	va_start(va, fmt);
	fprintf(stderr, "nm: %s: ", filename);
	if (membname)
		fprintf(stderr, "%s: ", membname);
	vfprintf(stderr, fmt, va);
	putc('\n', stderr);
	va_end(va);

	status = EXIT_FAILURE;
}

static void
cleanup(void)
{
	if (status != EXIT_FAILURE)
		remove(output);
}

static Symbol *
lookup(Objsym *osym)
{
	char *s;
	unsigned h;
	Symbol *sym;
	char *name = osym->name;

	h = 0;
	for (s = name; *s; s++)
		h += *s;
	h %= NR_SYMBOL;

	for (sym = symtab[h]; sym; sym = sym->hash) {
		if (!strcmp(name, sym->name))
			return sym;
	}

	if ((sym = malloc(sizeof(*sym))) == NULL) {
		error("out of memory");
		exit(EXIT_FAILURE);
	}

	sym->obj = NULL;
	sym->name = osym->name;
	sym->hash = symtab[h];
	symtab[h] = sym;

	refhead.next->prev = sym;
	sym->next = refhead.next;
	refhead.next = sym;
	sym->prev = &refhead;

	return sym;
}

static Symbol *
define(Objsym *osym, Obj *obj)
{
	Symbol *sym = lookup(osym);

	if (sym->obj) {
		error("%s: symbol redefined", osym->name);
		return NULL;
	}

	sym->obj = obj;
	sym->sym = osym;

	sym->next->prev = sym->prev;
	sym->prev->next = sym->next;
	sym->next = sym->prev = NULL;

	return sym;
}

static int
newsym(Objsym *osym, void *obj)
{
	switch (osym->type) {
	case 'U':
		lookup(osym);
	case '?':
	case 'N':
		break;
	default:
		if (isupper(osym->type))
			define(osym, obj);
		break;
	}

	return 1;
}

static void
load(Obj *obj)
{
	Objlst *lst;

	if ((lst = malloc(sizeof(*lst))) == NULL) {
		error("out of memory");
		return;
	}

	lst->obj = obj;
	lst->next = NULL;

	if (!objlast)
		objlast = objhead = lst;
	else
		objlast = objlast->next = lst;

	forsym(obj, newsym, obj);
}

static void
newobject(FILE *fp, int type, int inlib)
{
	Obj *obj;
	Symbol *sym, *p;

	if ((obj = objnew(type)) == NULL) {
		error("out of memory");
		return;
	}

	if (objread(obj, fp) < 0) {
		error("object file corrupted");
		goto delete;
	}

	if (inlib) {
		p = &refhead;
		for (sym = p->next; sym != p; sym = sym->next) {
			if (objlookup(obj, sym->name, 0))
				break;
		}
		if (sym == p)
			goto  delete;
	}
	load(obj);

	return;

delete:
	objdel(obj);
	return;
}

static int
newmember(FILE *fp, char *name, void *data)
{
	int t;

	membname = data;

	if ((t = objtype(fp, NULL)) == -1)
		return 1;
	newobject(fp, t, INLIB);

	return 1;
}

static int
newidx(Objsymdef *def, void *data)
{
	int t;
	Symbol *sym, *p;
	FILE *fp = data;

	p = &refhead;
	if (p->next == p)
		return 0;

	for (sym = p->next; sym != p; sym = sym->next) {
		if (strcmp(sym->name, def->name))
			continue;

		if (fseek(fp, def->offset, SEEK_SET) == EOF) {
			error(errstr());
			return 0;
		}

		if ((t = objtype(fp, NULL)) == -1) {
			error("library file corrupted");
			return 0;
		}

		newobject(fp, t, OUTLIB);
		return 1;
	}

	return 0;
}

static int
newlibrary(FILE *fp)
{
	if (foridx(fp, newidx, NULL))
		return 1;
	return formember(fp, newmember, NULL);
}

static FILE *
openfile(char *name, char *buffer)
{
	size_t pathlen, len;
	FILE *fp;
	char **bp, **base, **end;
	char libname[FILENAME_MAX];

	filename = name;
	membname = NULL;
	if (name[0] != '-' || name[1] != 'l') {
		if ((fp = fopen(name, "rb")) == NULL)
			error(errstr());
		return fp;
	}

	len = strlen(name+2) + 3;
	if (len > FILENAME_MAX-1) {
		error("library name too long");
		return NULL;
	}
	strcat(strcpy(buffer, "lib"), name+2);

	filename = buffer;
	if ((fp = fopen(libname, "rb")) != NULL)
		return fp;

	base = syslibs;
	end = &syslibs[MAX_LIB_PATHS];
	for (bp = base; bp < end && *bp; ++bp) {
		pathlen = strlen(*bp);
		if (pathlen + len > FILENAME_MAX-1)
			continue;
		memcpy(libname, *bp, pathlen);
		memcpy(libname+pathlen+1, buffer, len);
		buffer[pathlen] = '/';

		if ((fp = fopen(buffer, "rb")) != NULL)
			return fp;
	}

	error("not found");
	return NULL;
}

static void
pass1(int argc, char *argv[])
{
	int t;
	FILE *fp;
	char buff[FILENAME_MAX];

	for ( ; *argv; ++argv) {
		if ((fp = openfile(*argv, buff)) == NULL)
			continue;

		if ((t = objtype(fp, NULL)) != -1)
			newobject(fp, t, OUTLIB);
		else if (archive(fp))
			newlibrary(fp);
		else
			error("bad format");

		fclose(fp);
	}

	if (refhead.next != &refhead) {
		Symbol *sym, *p;

		p = &refhead;
		for (sym = p->next; sym != p; sym = sym->next) {
			fprintf(stderr,
			        "ld: symbol '%s' not defined\n",
			        sym->name);
		}
		exit(EXIT_FAILURE);
	}
}

static void
pass2(int argc, char *argv[])
{
}

static void
usage(void)
{
	fputs("usage: ld [options] file ...\n", stderr);
	exit(EXIT_FAILURE);
}

static void
Lpath(char *path)
{
	char **bp, **base, **end;

	base = syslibs;
	end = &syslibs[MAX_LIB_PATHS];
	for (bp = base; bp < end && *bp; ++bp)
		;
	if (bp == end) {
		fputs("ld: too many -L options\n", stderr);
		exit(1);
	}
	*bp = path;
}

static void
refer(char *name)
{
	Objsym *osym;

	if ((osym = malloc(sizeof(*osym))) == NULL) {
		fputs("ld: out of memory\n", stderr);
		return;
	}

	osym->name = name;
	osym->type = 'U';
	osym->size = osym->value = 0;
	osym->next = osym->hash = NULL;

	lookup(osym);
}

int
main(int argc, char *argv[])
{
	char *cp, **p;

	for (--argc; *++argv; --argc) {
		if (argv[0][0] != '-' || argv[0][1] == 'l')
			break;
		if (argv[0][1] == '-') {
			--argc, ++argv;
			break;
		}
		for (cp = &argv[0][1]; *cp; ++cp) {
			switch (*cp) {
			case 's':
				sflag = 1;
				break;
			case 'x':
				xflag = 1;
				break;
			case 'X':
				Xflag = 1;
				break;
			case 'r':
				rflag = 1;
				break;
			case 'd':
				dflag = 1;
				break;
			case 'i':
			case 'n':
				/* TODO */
				break;
			case 'L':
				if (argc == 0)
					goto usage;
				++argv, --argc;
				Lpath(*argv);
				break;
			case 'u':
				if (argc == 0)
					goto usage;
				++argv, --argc;
				refer(*argv);
				break;
			case 'o':
				if (argc == 0)
					goto usage;
				++argv, --argc;
				output = *argv;
				break;
			case 'e':
				if (argc == 0)
					goto usage;
				++argv, --argc;
				entry = *argv;
				break;
			case 'D':
				if (argc == 0)
					goto usage;
				++argv, --argc;
				datasiz = *argv;
				break;
			default:
			usage:
				usage();
			}
		}
	}

	if (argc == 0)
		usage();

	atexit(cleanup);

	pass1(argc, argv);
	pass2(argc, argv);

	return status;
}
