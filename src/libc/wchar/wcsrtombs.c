#include <string.h>
#include <limits.h>
#include <wchar.h>

#undef wcsrtombs

size_t
wcsrtombs(char *restrict dest, const wchar_t **restrict src,
          size_t len, mbstate_t *restrict ps)
{
	size_t cnt, n;
	wchar_t wc;
	char buf[MB_LEN_MAX];
	static mbstate_t p;

	if (!ps)
		ps = &p;

	for (n = 0; ; n += cnt) {
		wc = **src;
		cnt = wcrtomb(buf, wc, ps);
		if (cnt == (size_t) -1)
			return -1;

		if (dest) {
			if (n+cnt > len)
				return n;
			memcpy(dest+n, buf, cnt);
		}
		*src += 1;

		if (wc == L'\0')
			break;
	}
	*src = NULL;

	return n;
}
