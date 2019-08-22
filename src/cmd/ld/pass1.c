#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <scc/mach.h>
#include <scc/scc.h>
#include <scc/ar.h>

#include "ld.h"

enum {
	OUTLIB,
	INLIB,
};

int bintype = -1;
static Symbol refhead = {
	.next = &refhead,
	.prev = &refhead,
};

Symbol defhead = {
	.next = &defhead,
	.prev = &defhead,
};
static Objlst *objlast;

Objlst *objhead;

static Symbol *
linksym(Symbol *list, Symbol *sym)
{
	sym->next = list;
	sym->prev = list->prev;
	list->prev->next = sym;
	list->prev = sym;
	return sym;
}

static Symbol *
unlinksym(Symbol *sym)
{
	sym->next->prev = sym->prev;
	sym->prev->next = sym->next;
	return sym;
}

static Symbol *
undef(char *name)
{
	return linksym(&refhead, install(name));
}

static Symbol *
define(Objsym *osym, Obj *obj)
{
	Symbol *sym = lookup(osym->name);

	if (!sym) {
		sym = undef(osym->name);
	} else if (sym->def && sym->def->type != 'C') {
		error("%s: symbol redefined", osym->name);
		return NULL;
	}

	/* TODO: deal with C symbols */

	sym->obj = obj;
	sym->def = osym;
	sym->size = osym->size;
	sym->value = osym->value;

	unlinksym(sym);
	linksym(&defhead, sym);

	return sym;
}

static int
moreundef(void)
{
	return refhead.next != &refhead;
}

static void
listundef(void)
{
	Symbol *sym;

	for (sym = refhead.next; sym != &refhead; sym = sym->next) {
		fprintf(stderr,
		        "ld: symbol '%s' not defined\n",
		        sym->name);
	}
}

static int
is_needed(Obj *obj)
{
	Symbol *sym;

	for (sym = refhead.next; sym != &refhead; sym = sym->next) {
		if (objlookup(obj, sym->name, 0))
			return 1;
	}

	return 0;
}


static int
newsym(Objsym *osym, Obj *obj)
{
	Symbol *sym;

	switch (osym->type) {
	case 'U':
		if ((sym = lookup(osym->name)) == NULL)
			sym = undef(osym->name);
		break;
	case '?':
	case 'N':
		break;
	case 'C':
		sym = lookup(osym->name);
		if (!sym || !sym->def) {
			sym = define(osym, obj);
			break;
		}
		if (sym->def->type != 'C')
			break;
		if (sym->size < osym->size)
			sym->size = osym->size;
		break;
	default:
		if (isupper(osym->type))
			define(osym, obj);
		break;
	}

	return 1;
}

static void
addobj(Obj *obj, FILE *fp)
{
	int n;
	Objlst *lst;
	Objsym *sym;
	Objsect *secp;

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

	for (sym = obj->syms; sym; sym = sym->next)
		newsym(sym, obj);
}

static void
newobject(FILE *fp, int type, int inlib)
{
	Obj *obj;
 
	if ((obj = objnew(type)) == NULL) {
		error("out of memory");
		return;
	}
 
	if (bintype != -1 && bintype != type) {
		error("not compatible object file");
		goto delete;
	}
	bintype = type;
 
	if ((*obj->read)(obj, fp) < 0) {
		error("object file corrupted");
		goto delete;
	}

	if (inlib && !is_needed(obj))
		goto delete;

	addobj(obj, fp);
	return;

 delete:
	objdel(obj);
	return;
}

static void
addlib(FILE *fp)
{
	int t, added;
	long n;
	Objsymdef *def, *dp;
	Symbol *sym;

	if (getindex(bintype, &n, &def, fp) < 0) {
		error("corrupted index");
		return;
	}

	for (added = 0; moreundef(); added = 0) {
		for (dp = def; dp; dp = dp->next) {
			sym = lookup(dp->name);
			if (!sym || sym->def)
				continue;

			if (fseek(fp, dp->offset, SEEK_SET) == EOF) {
				error(errstr());
				goto clean;
			}

			if ((t = objtype(fp, NULL)) == -1) {
				error("library file corrupted");
				goto clean;
			}

			if (t != bintype) {
				error("incompatible library");
				goto clean;
			}

			newobject(fp, t, OUTLIB);
			added = 1;
		}

		if (!added)
			break;
	}
clean:
	free(def);
}

static int
newmember(FILE *fp, char *name, void *data)
{
	int t;
	int *nmemb = data;

	if (bintype == -1) {
		error("an object file is needed before any library");
		return 0;
	}

	if (*nmemb++ == 0) {
		if (!strncmp(name, "/", SARNAM) ||
		    !strncmp(name, "__.SYMDEF", SARNAM)) {
			addlib(fp);
			return 0;
		}
	}

	membname = name;
	if ((t = objtype(fp, NULL)) == -1)
		return 1;

	if (bintype != t) {
		error("wrong object file format");
		return 1;
	}

	newobject(fp, t, INLIB);
	membname = NULL;

	return 1;
}

static int
newlibrary(FILE *fp)
{
	int nmemb = 0;

	return formember(fp, newmember, &nmemb);
}

static FILE *
openfile(char *name, char *buffer)
{
	size_t pathlen, len;
	FILE *fp;
	char **bp;
	char libname[FILENAME_MAX];
	extern char *syslibs[];

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

	for (bp = syslibs; *bp; ++bp) {
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
load(char *name)
{
	int t;
	FILE *fp;
	char buff[FILENAME_MAX];

	if ((fp = openfile(name, buff)) == NULL)
		return;

	if ((t = objtype(fp, NULL)) != -1)
		newobject(fp, t, OUTLIB);
	else if (archive(fp))
		newlibrary(fp);
	else
		error("bad format");
}

/*
 * Get the list of object files that are going to be linked
 */
void
pass1(int argc, char *argv[])
{
	char **ap, *cp, *arg;

	for (ap = argv+1; *ap; ++ap) {
		if (ap[0][0] != '-') {
			load(*ap);
			continue;
		}
		for (cp = &ap[0][1]; *cp; ++cp) {
			switch (*cp) {
			case 'l':
				arg = (cp[1]) ? cp+1 : *++ap;
				load(arg);
				continue;
			case 'u':
				arg = (cp[1]) ? cp+1 : *++ap;
				undef(arg);
				continue;
			}
		}
	}

	if (moreundef()) {
		listundef();
		exit(EXIT_FAILURE);
	}
}
