#include <stdio.h>
#include <wchar.h>

#undef wcsnlen

size_t
wcsnlen(const wchar_t *s, size_t maxlen)
{
	return 0;
}
