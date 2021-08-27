#include <string.h>

#undef strncat

char *
strncat(char *restrict s1, const char *restrict s2, size_t n)
{
	char *ret = s1;

	while(*s1)
		++s1;

	while (n-- > 0 && *s2)
		*s1++ = *s2++;
	*s1 = '\0';

	return ret;
}
