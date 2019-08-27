#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <scc/arg.h>
#include <scc/mach.h>

static int status;
static char tmpname[FILENAME_MAX];
static char *filename;
char *argv0;

static void
error(char *fmt, ...)
{
	va_list va;

	va_start(va, fmt);
	fprintf(stderr, "strip: %s: ", filename);
	vfprintf(stderr, fmt, va);
	putc('\n', stderr);
	va_end(va);

	status = EXIT_FAILURE;
}

static void
doit(char *fname)
{
	int type;
	size_t r;
	FILE *fp;
	Obj *obj;

	filename = fname;
	if ((fp = fopen(fname, "rb")) == NULL)
		goto err1;
	if ((type = objtype(fp, NULL)) < 0)
		goto err2;
	if ((obj = newobj(type)) == NULL)
		goto err2;
	if (readobj(obj, fp) < 0)
		goto err3;
	fclose(fp);

	if (strip(obj) < 0)
		goto err2;

	r = snprintf(tmpname, sizeof(tmpname), "%s.tmp", fname);
	if (r >= sizeof(tmpname)) {
		errno = ERANGE;
		goto err2;
	}

	if ((fp = fopen(tmpname, "wb")) == NULL)
		goto err2;

	if (writeobj(obj, fp) < 0)
		goto err3;
	fclose(fp);
	delobj(obj);

	if (rename(tmpname, fname) == EOF)
		goto err1;

	return;

err3:
	fclose(fp);
err2:
	delobj(obj);
err1:
	error(strerror(errno));
	remove(tmpname);
}

static void
usage(void)
{
	fputs("usage: strip [file...]\n", stderr);
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
		doit("a.out");
	} else {
		for (; *argv; ++argv)
			doit(*argv);
	}

	return status;
}
