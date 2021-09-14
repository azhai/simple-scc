#include <errno.h>
#include <stddef.h>

#include "../../syscall.h"

static char heap[16384];

/*
 * TODO: Temporary solution until we implement mmap in Darwin
 *       because it does not support any form of brk().
 */
void *
_getheap(void)
{
	return heap;
}

int
_brk(void *addr)
{
	static char *cur = heap;
	char *p = addr;

	if (p < heap || p > &heap[sizeof(heap) - 1]) {
		errno = ENOMEM;
		return -1;
	}
	cur = p;

	return 0;
}
