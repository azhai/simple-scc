#include <string.h>
#undef strspn

size_t
strspn(const char *s1, const char *s2)
{
	char buf[128];
	unsigned char ch;
	size_t n;

	memset(buf, 0, sizeof(buf));
	while (ch = *s2++)
		buf[ch] = 1;

	for (n = 0; (ch = *s1++) && buf[ch]; ++n)
		;

	return n;
}
