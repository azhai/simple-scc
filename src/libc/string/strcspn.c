#include <string.h>

#undef strcspn

size_t
strcspn(const char *s1, const char *s2)
{
	const unsigned char *s = s1;
	const unsigned char *reject = s2;
	size_t n;
	unsigned ch;
	char map[__NUMCHARS] = {0};

	while (ch = *reject++)
		map[ch] = 1;

	for (n = 0; (ch = *s++) && !map[ch]; ++n)
		;

	return n;
}
