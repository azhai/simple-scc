#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <scc/arg.h>
#include <scc/mach.h>

static int status;
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
strip(char *fname)
{
	int type;
	FILE *fp;
	Obj *obj;
	Objops *ops;

	errno = 0;
	filename = fname;

	if ((fp = fopen(fname, "rb")) == NULL)
		goto err1;

	if ((type = objtype(fp, NULL)) < 0) {
		error("file format not recognized");
		goto err2;
	}
	if ((obj = objnew(type)) == NULL) {
		error("out of memory");
		goto err3;
	}
	ops = obj->ops;

	if ((*ops->read)(obj, fp) < 0) {
		error("file corrupted");
		goto err3;
	}
	fclose(fp);
	fp = NULL;

	if ((*ops->strip)(obj) < 0) {
		error("error stripping");
		goto err3;
	}

	/* TODO: Use a temporary file */
	if ((fp = fopen(fname, "wb")) == NULL)
		goto err1;

	if (objwrite(obj, fp) < 0) {
		error("error writing output");
		goto err3;
	}

	fclose(fp);
	objdel(obj);

	return;

err3:
	objdel(obj);
err2:
	if (fp)
		fclose(fp);
err1:
	if (errno)
		error(strerror(errno));

	return;
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
		strip("a.out");
	} else {
		for (; *argv; ++argv)
			strip(*argv);
	}

	return status;
}
