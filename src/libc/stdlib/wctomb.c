#include <stdlib.h>
#include <wchar.h>

#undef wctomb

int
wctomb(char *s, wchar_t wc)
{
	return wcrtomb(s, wc, NULL);
}
