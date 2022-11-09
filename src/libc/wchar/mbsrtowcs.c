#include <limits.h>
#include <wchar.h>

#undef mbsrtowcs

size_t
mbsrtowcs(wchar_t *restrict dest, const char **restrict src, size_t len,
          mbstate_t *restrict ps)
{
	wchar_t wc;
	size_t cnt, n;
	static mbstate_t p;

	if (!ps)
		ps = &p;

	for (n = 0; ; n++) {
		cnt = mbrtowc(&wc, *src, MB_LEN_MAX, ps);
		if (cnt == (size_t) -1)
			return -1;

		if (dest) {
			if (n == len)
				return n;
			*dest++ = wc;
		}
		*src += cnt;

		if (wc == L'\0')
			break;
	}
	*src = NULL;

	return n;
}
