#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include <scc/arg.h>
#include <scc/scc.h>

void
fatal(char *fname, int lineno, char *fmt, ...)
{
	va_list va;

	va_start(va, fmt);
	fprintf(stderr, "%s:", argv0);
	vfprintf(stderr, fmt, va);
	fputc('\n', stderr);
	abort();
}
