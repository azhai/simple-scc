#include <string.h>

#undef memmove

void *
memmove(void *s1, const void *s2, size_t n)
{
	char *d = s1;
	const char *s = s2;

	if (d < s) {
		while (n-- > 0)
			*d++ = *s++;
	} else {
		s += n-1, d += n-1;

		while (n-- > 0)
			*d-- = *s--;
	}
	return s1;
}
