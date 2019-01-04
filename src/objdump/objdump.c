#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <scc/arg.h>
#include <scc/mach.h>

char *argv0;
static char *filename;
static int status;

static void
error(char *fmt, ...)
{
	va_list va;

	va_start(va, fmt);
	fprintf(stderr, "objdump: %s: ", filename);
	vfprintf(stderr, fmt, va);
	putc('\n', stderr);
	va_end(va);

	status = EXIT_FAILURE;
}

static void
dump(char *fname)
{
}

static void
usage(void)
{
	fputs("usage: objdump file ...\n", stderr);
	exit(1);
}

int
main(int argc, char *argv[])
{
	ARGBEGIN {
	default:
		usage();
	} ARGEND

	if (argc == 0)
		dump("a.out");
	else while (*argv) {
		dump(*argv++);
	}

	if (fclose(stdout) == EOF) {
		fprintf(stderr,
		        "objdump: writing output: %s\n",
		        strerror(errno));
		return EXIT_FAILURE;
	}

	return status;
}
