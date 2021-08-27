#include <string.h>

#undef strncpy

char *
strncpy(char *restrict s1, const char *restrict s2, size_t n)
{
	char *ret = s1;

	for (; n > 0 && *s2; --n)
		*s1++ = *s2++;

	while (n-- > 0)
		*s1++ = '\0';

	return ret;
}
