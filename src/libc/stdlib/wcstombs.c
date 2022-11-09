#include <stdlib.h>
#include <wchar.h>

#undef wcstombs

size_t
wcstombs(char *restrict dest, const wchar_t *restrict src, size_t n)
{
	return wcsrtombs(dest, (void *) &src, n, NULL);
}
