#include <stdlib.h>

#undef mblen

int
mblen(const char *s, size_t n)
{
	return mbtowc(NULL, s, n);
}
