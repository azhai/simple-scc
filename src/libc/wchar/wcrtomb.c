#include <errno.h>
#include <wchar.h>

#include "../libc.h"

#undef wcrtomb

size_t
wcrtomb(char *restrict s, wchar_t wc, mbstate_t *restrict ps)
{
	int i, n;
	unsigned long c = wc;
	static unsigned long limits[] = {
		0x80, 0x800, 0x10000, 0x200000, 0x4000000
	};

	if (!s)
		return 1;

	if (_validutf8(wc)) {
		errno = EILSEQ;
		return -1;
	}

	for (n = 0; n < 5 && c >= limits[n]; ++n)
		;

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
