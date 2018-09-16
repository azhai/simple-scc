#include <string.h>
#undef memcmp

int
memcmp(const void *s1, const void *s2, size_t n)
{
	const unsigned char *s = (unsigned char *) s1;
	const unsigned char *t = (unsigned char *) s2;

	for ( ; n > 0 && *s++ == *t++; --n)
		;
	return n ? (*s - *t) : 0;
}
