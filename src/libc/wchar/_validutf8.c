#include <wchar.h>

#include "../libc.h"

int
_validutf8(wchar_t wc)
{
	if (wc >= 0xD800 && wc < 0xDBFF)
		return 0;
	if (wc >= 0xDC00 && wc < 0xDCFF)
		return 0;
	if (wc > 0x10FFFF)
		return 0;
	return 1;
}
