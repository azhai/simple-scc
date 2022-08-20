#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#undef assert

void
assert(int exp)
{
	if (exp)
		return;
	fputs("assert failed\n", stderr);
	abort();
}
