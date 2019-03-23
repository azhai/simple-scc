#include <stdarg.h>
#include <stdio.h>

#include <scc/scc.h>

int debug;

void
dbg(const char *fmt, ...)
{
	if (!debug)
		return;
	va_list va;
	va_start(va, fmt);
	vfprintf(stderr, fmt, va);
	putc('\n', stderr);
	va_end(va);
	return;
}
