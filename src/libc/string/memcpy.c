#include <string.h>

#undef memcpy

void *
memcpy(void *restrict s1, const void *restrict s2, size_t n)
{
	char *d = s1;
	const char *s = s2;

	while (n-- > 0)
		*d++ = *s++;

	return s1;
}
