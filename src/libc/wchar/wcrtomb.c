#include <errno.h>
#include <wchar.h>

#include "../libc.h"

#undef wcrtomb

size_t
wcrtomb(char *restrict s, wchar_t wc, mbstate_t *restrict ps)
{
	int i, n;
	unsigned long c = wc;

	if (!s)
		return 1;

	if (!_validutf8(wc, &n)) {
		errno = EILSEQ;
		return -1;
	}

	n--;
	*s = 0;
	for (i = 0; i < n; i++) {
		*s >>= 1;
		*s |= 0x80;

		s[n-i] = 0x80 | (c & 0x3f);
		c >>= 6;
	}
	*s |= c;

	return n+1;
}
