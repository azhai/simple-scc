#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <scc/mach.h>
#include <scc/scc.h>
#include <scc/ar.h>

#include "ld.h"

static int bintype = -1;
Objlst *objhead, *objlast;

static Symbol *
define(Objsym *osym, Obj *obj)
{
	Symbol *sym = lookup(osym->name);

	if (!sym) {
		sym = install(osym->name);
	} else if (sym->def && sym->def->type != 'C') {
		error("%s: symbol redefined", osym->name);
		return NULL;
	}

	sym->obj = obj;
	sym->def = osym;
	sym->size = osym->size;
	sym->value = osym->value;

	sym->next->prev = sym->prev;
	sym->prev->next = sym->next;
	sym->next = sym->prev = NULL;

	return sym;
}

static int
newsym(Objsym *osym, Obj *obj)
{
	Symbol *sym;

	switch (osym->type) {
	case 'U':
		if ((sym = lookup(osym->name)) == NULL)
			sym = install(osym->name);
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
 
	if (objread(obj, fp) < 0) {
		error("object file corrupted");
		goto delete;
	}
 
	if (objsyms(obj) < 0 || objsect(obj) < 0) {
		error("object file corrupted");
		goto delete;
	}

	/*
	 * we add the object to the list of objects
	 * if we are not in a library without index,
	 * or in that case if the library defines
	 * some symbol needed.
	 */
	if (!inlib || defasym(obj)) {
		addobj(obj, fp);
		return;
	}

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

	added = 1;
	while (moreundef() && added) {
		added = 0;
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

/*
 * Get the list of object files that are going to be linked
 */
void
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
	}

	if (moreundef()) {
		listundef();
		exit(EXIT_FAILURE);
	}
}
