#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

void
__assert(char *exp, char *file, long line)
{
	fprintf(stderr, "%s:%ld: assertion failed '%s'\n", file, line, exp);
	abort();
}
