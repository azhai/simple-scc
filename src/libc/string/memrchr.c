#include <string.h>

#undef memrchr

void *
memrchr(const void *s, int c, size_t n)
{
	unsigned char *p = s;

	for (p += n-1; n > 0 && *p != c; --p)
		n--;
	return (n == 0) ? NULL : p;
}
