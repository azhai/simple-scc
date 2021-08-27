#include <string.h>

#undef strspn

size_t
strspn(const char *s1, const char *s2)
{
	const unsigned char *s = s1;
	const unsigned char *accept = s2;
	size_t n;
	unsigned ch;
	char map[__NUMCHARS] = {0};

	while ((ch = *accept++) != '\0')
		map[ch] = 1;

	for (n = 0; (ch = *s++) != '\0' && map[ch]; ++n)
		;

	return n;
}
