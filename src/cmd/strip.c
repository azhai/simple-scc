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
	FILE *fp, *tmp;
	Obj *obj;

	filename = fname;
	if ((tmp = fopen("strip.tmp", "wb")) == NULL) {
		error("%s", strerror(errno));
		goto err;
	}
	if ((fp = fopen(fname, "rb")) == NULL) {
		error("%s", strerror(errno));
		goto err1;
	}
	if ((type = objtype(fp, NULL)) < 0) {
		error("file format not recognized");
		goto err2;
	}
	if ((obj = objnew(type)) == NULL) {
		error("out of memory");
		goto err3;
	}
	if (objread(obj, fp) < 0) {
		error("file corrupted");
		goto err3;
	}
	objstrip(obj);

	if (objwrite(obj, tmp) < 0) {
		error("error writing output");
		goto err3;
	}

	fclose(fp);
	fp = NULL;

	if (remove(fname) || rename("strip.tmp", fname)) {
		error(strerror(errno));
		goto err3;
	}
	objdel(obj);
	return;

err3:
	objdel(obj);
err2:
	if (fp)
		fclose(fp);
err1:
	fclose(tmp);
	remove("strip.tmp");
err:
	return;
}

static void
usage(void)
{
	fputs("usage: strip file ...\n", stderr);
	exit(EXIT_FAILURE);
}

int
main(int argc, char *argv[])
{
	ARGBEGIN {
	default:
		usage();
	} ARGEND

	if (argc == 1)
		usage;

	for (argc--; argc > 0; argc--)
		strip(*argv++);
	return status;
}
