#include <string.h>

#undef strcat

char *
strcat(char *restrict s1, const char *restrict s2)
{
	char *ret = s1;

	while (*s1)
		++s1;

	while ((*s1++ = *s2++) != 0)
		;

	return ret;
}
