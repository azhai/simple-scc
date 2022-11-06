#include <wchar.h>

#include "../libc.h"

#undef mbrtowc

size_t
mbrtowc(wchar_t *restrict pwc, const char *restrict s, size_t n,
        mbstate_t *restrict ps)
{
	unsigned char *t = (unsigned char *) s;
	unsigned long wc;
	unsigned c;
	size_t i, len;

	if (s == NULL)
		return 0;

	wc = c = *t++;
	for (len = 0; n > 0 && c & 0x80; --n, ++len)
		c <<= 1;
	if (n == 0 || len == 1 || len == 8)
		return -1;
	if (len == 0)
		goto return_code;

	wc = (c & 0xFF) >> len;
	for (i = 0; i < len-1; i++) {
		if (((c = *t++) & 0xC0) != 0x80)
			return -1;
		wc <<= 6;
		wc |= c & 0x3F;
	}

	if (!_validutf8(wc))
		return -1;

return_code:
	if (pwc)
		*pwc = wc;
	if (*s == '\0')
		return 0;
	return t - (unsigned char *) s;
}
