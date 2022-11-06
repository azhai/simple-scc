#include <limits.h>
#include <wchar.h>

#undef mbsrtowcs

size_t
mbsrtowcs(wchar_t *restrict dest, const char **restrict src, size_t len,
          mbstate_t *restrict ps)
{
	wchar_t wc;
	size_t cnt, n;

	for (n = 0; !dest || n < len; n++) {
		cnt = mbrtowc(&wc, *src, MB_LEN_MAX, ps);
		if (cnt == (size_t) -1)
			return -1;
		*src += cnt;

		if (dest)
			*dest++ = wc;
		if (wc == L'\0')
			break;
	}
	*src = NULL;

	return n;
}
