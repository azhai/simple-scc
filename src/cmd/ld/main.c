#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <scc/syslibs.h>

#include "ld.h"

char *output = "a.out", *entry = "start";

char *filename, *membname;

int sflag;        /* discard all the symbols */
int xflag;        /* discard local symbols */
int Xflag;        /* discard locals starting with 'L' */
int rflag;        /* preserve relocation bits */
int dflag;        /* define common even with rflag */
int gflag;        /* preserve debug symbols */
char *Dflag;      /* size of data */

static int status;

char *
errstr(void)
{
	return strerror(errno);
}

void
error(char *fmt, ...)
{
	va_list va;

	va_start(va, fmt);
	fprintf(stderr, "ld: %s: ", filename);
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

/*
 * pass1: Get the list of object files that are going to be linked.
 * pass2: Calculate the size of every segment.
 * pass3: Rebase all symbols in sections
 */
static void
ld(int argc, char*argv[])
{
	pass1(argc, argv);
	pass2(argc, argv);
	pass3(argc, argv);
	debugsym();
	debugsec();
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
	char **bp, **end;

	end = &syslibs[MAX_LIB_PATHS];
	for (bp = syslibs; bp < end && *bp; ++bp)
		;
	if (bp == end) {
		fputs("ld: too many -L options\n", stderr);
		exit(1);
	}
	*bp = path;
}

int
main(int argc, char *argv[])
{
	int files = 0;
	char *cp, *arg, **ap;

	for (ap = argv+1; *ap; ++ap) {
		if (ap[0][0] != '-') {
			files = 1;
			continue;
		}
		for (cp = &ap[0][1]; *cp; ++cp) {
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
			case 'u':
			case 'l':
				arg = (cp[1]) ? cp+1 : *++ap;
				if (!arg)
					goto usage;
				goto next_arg;
			case 'L':
				arg = (cp[1]) ? cp+1 : *++ap;
				if (!arg)
					goto usage;
				Lpath(arg);
				goto next_arg;
			case 'o':
				arg = (cp[1]) ? cp+1 : *++ap;
				if (!arg)
					goto usage;
				output = arg;
				goto next_arg;
			case 'e':
				arg = (cp[1]) ? cp+1 : *++ap;
				if (!arg)
					goto usage;
				entry = arg;
				goto next_arg;
			case 'D':
				arg = (cp[1]) ? cp+1 : *++ap;
				if (!arg)
					goto usage;
				Dflag = arg;
				goto next_arg;
			default:
			usage:
				usage();
			}
		}

		next_arg:
			continue;
	}

	if (!files)
		usage();

	atexit(cleanup);
	ld(argc, argv);

	return status;
}
