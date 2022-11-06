#include <stdlib.h>
#include <wchar.h>

#undef mbtowc

int
mbtowc(wchar_t *restrict pwc, const char *restrict s, size_t n)
{
	return mbrtowc(pwc, s, n, NULL);
}
