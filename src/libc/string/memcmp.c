#include <string.h>

#undef memcmp

int
memcmp(const void *s1, const void *s2, size_t n)
{
	const unsigned char *s = s1;
	const unsigned char *t = s2;

	for (; n > 0 && *s == *t; --n)
		++s, ++t;

	return (n > 0) ? *s - *t : 0;
}
