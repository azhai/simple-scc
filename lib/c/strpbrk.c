#include <string.h>
#undef strpbrk

char *
strpbrk(const char *s1, const char *s2)
{
	char buf[128];
	unsigned ch;

	memset(buf, 0, sizeof(buf));
	while (ch = *s2++)
		buf[ch] = 1;

	while ((ch = *s1) && !buf[ch])
		s1++;

	return (ch == '\0') ? NULL : (char *) s1;
}
