#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <scc/ar.h>
#include <scc/arg.h>
#include <scc/mach.h>

#include "sys.h"

#define NR_SYMDEF 32

static long nsymbols;
static int status, artype, nolib;
static char *filename, *membname;
static Objsymdef *htab[NR_SYMDEF], *head;
static long offset;
char *argv0;

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
	fprintf(stderr, "ranlib: %s: ", filename);
	if (membname)
		fprintf(stderr, "%s: ", membname);
	vfprintf(stderr, fmt, va);
	putc('\n', stderr);
	va_end(va);

	status = EXIT_FAILURE;
}

Objsymdef *
lookup(char *name)
{
	unsigned h;
	Objsymdef *dp;
	char *s;
	size_t len;

	h = 0;
	for (s = name; *s; s++)
		h += *s;
	h %= NR_SYMDEF;

	for (dp = htab[h]; dp; dp = dp->next) {
		if (!strcmp(dp->name, name))
			return dp;
	}

	len = strlen(name) + 1;
	dp = malloc(sizeof(*dp));
	s = malloc(len);
	if (!dp || !s) {
		free(s);
		free(dp);
		return NULL;
	}

	nsymbols++;
	dp->name = s;
	memcpy(dp->name, name, len);
	dp->type = 'U';
	dp->offset = -1;
	dp->hash = htab[h];
	htab[h] = dp;
	dp->next = head;
	head = dp;

	return dp;
}

static int
newsymbol(Objsym *sym, void *data)
{
	Objsymdef *np;

	if (!isupper(sym->type) || sym->type == 'N')
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
newmember(FILE *fp, char *nam, void *data)
{

	int t, ret = 0;
	Obj *obj;

	if (artype == -1 && (!strcmp(nam, "/") || !strcmp(nam, "__.SYMDEF")))
		return 1;

	membname = nam;
	offset = ftell(fp);

	if (offset == EOF) {
		error(errstr());
		return 0;
	}

	t = objtype(fp, NULL);
	if (t == -1 || artype != -1 && artype != t) {
		nolib = 1;
		return 0;
	}
	artype = t;

	if ((obj = objnew(t)) == NULL) {
		error("out of memory");
		return 0;
	}

	if (objread(obj, fp) < 0) {
		error("file corrupted");
		goto error;
	}

	if (!forsym(obj, newsymbol, NULL)) {
		error("traversing object file");
		goto error;
	}

	ret = 1;

error:
	objdel(obj);
	return ret;
}

static void
freehash(void)
{
	Objsymdef **npp, *next, *np;

	for (npp = htab; npp < &htab[NR_SYMDEF]; npp++)
		*npp = NULL;

	for (np = head; np; np = next) {
		next = np->next;
		free(np->name);
		free(np);
	}

	head = NULL;
}

static int
readsyms(FILE *fp)
{
	/* TODO: Change archive to returns -1 */
	if (!archive(fp)) {
		error("file format not recognized");
		return 0;
	}

	if (formember(fp, newmember, NULL) < 0) {
		error("while traversing archive");
		return 0;
	}

	return 1;
}

static int
merge(FILE *to, struct fprop *prop, FILE *lib, FILE *idx)
{
	int c;
	char mtime[13];
	struct ar_hdr first;

	rewind(lib);
	rewind(idx);
	fseek(lib, SARMAG, SEEK_SET);

	if (fread(&first, sizeof(first), 1, lib) != 1)
		return 0;

	/* TODO: This name depends of the format */
	if (!strncmp(first.ar_name, "/", SARNAM))
		fseek(lib, atol(first.ar_size), SEEK_CUR);

	fwrite(ARMAG, SARMAG, 1, to);

        strftime(mtime, sizeof(mtime), "%s", gmtime(&prop->time));
        fprintf(to,
                "%-16.16s%-12s%-6u%-6u%-8lo%-10ld`\n",
                "/",      /* TODO: This name depends of the format */
                mtime,
                prop->uid,
                prop->gid,
                prop->mode,
                prop->size);

	while ((c = getc(idx)) != EOF)
		putc(c, to);

	while ((c = getc(lib)) != EOF)
		putc(c, to);

	fflush(to);

	if (ferror(to) || ferror(lib) || ferror(idx))
		return 0;

	return 1;
}

static int
copy(FILE *from, char *fname)
{
	int c, ret;
	FILE *fp;

	if ((fp = fopen(fname, "wb")) == NULL)
		return 0;

	rewind(from);
	while ((c = getc(from)) != EOF)
		putc(c, fp);
	fflush(fp);

	ret = !ferror(fp) && !ferror(from);

	fclose(fp);

	return ret;
}

static void
ranlib(char *fname)
{
	int c;
	FILE *fp, *idx, *out;
	long siz;
	struct fprop prop;

	errno = 0;
	nolib = 0;
	artype = -1;
	nsymbols = 0;
	filename = fname;
	freehash();

	fp = fopen(fname, "rb");
	idx = tmpfile();
	out = tmpfile();
	if (!fp || !idx || !out)
		goto error;

	if (!readsyms(fp))
		goto error;

	if (nolib)
		goto error;

	siz = setindex(artype, nsymbols, head, idx);
	if (siz <= 0)
		goto error;

	if (getstat(fname, &prop) < 0)
		goto error;
	prop.size = siz;
	prop.time = time(NULL);

	if (!merge(out, &prop, fp, idx))
		goto error;

	fclose(fp);
	fclose(idx);
	fp = idx = NULL;

	if (!copy(out, fname))
		goto error;

	fclose(out);

	return;

error:
	if (errno)
		error(errstr());
	if (idx)
		fclose(idx);
	if (out)
		fclose(out);
	if (fp)
		fclose(fp);
}

static void
usage(void)
{
	fputs("usage: ranlib [-t] file...\n", stderr);
	exit(EXIT_FAILURE);
}

int
main(int argc, char *argv[])
{
	ARGBEGIN {
	case 't':
		break;
	default:
		usage();
	} ARGEND

	if (argc == 0)
		usage();

	for (; *argv; ++argv)
		ranlib(*argv);

	return status;
}
