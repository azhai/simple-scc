#include <stdlib.h>

#undef srand
#undef rand

static unsigned long next;

void
srand(unsigned seed)
{
	next = seed;
}

int
rand(void)
{
	/*
	 * next is just an arbitrary number
	 * used to generate a random number
	 * which is <= RAND_MAX
	 */
	next = next * 1103515245 + 12345;
	return (unsigned)(next/65536) % (RAND_MAX + 1);
}
