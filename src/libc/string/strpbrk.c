#include <string.h>

#undef strpbrk

char *
strpbrk(const char *s1, const char *s2)
{
	const unsigned char *s = (const unsigned char *) s1;
	const unsigned char *accept = (const unsigned char *) s2;
	unsigned ch;
	char map[__NUMCHARS] = {0};

	while ((ch = *accept++) != 0)
		map[ch] = 1;

	while ((ch = *s) != 0 && !map[ch])
		s++;

	return (ch == '\0') ? NULL : (char *) s;
}
