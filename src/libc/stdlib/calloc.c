#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#undef calloc

void *
calloc(size_t nmemb, size_t size)
{
	size_t nbytes;
	void *mem;

	if (!nmemb || !size || nmemb > SIZE_MAX/size)
                return NULL;

	nbytes = nmemb * size;
	if ((mem = malloc(nbytes)) == NULL)
		return NULL;
	return memset(mem, 0, nbytes);
}
