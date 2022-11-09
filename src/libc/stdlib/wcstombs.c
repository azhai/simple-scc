#include <stdlib.h>
#include <wchar.h>

#undef wcstombs

size_t
wcstombs(char *restrict dest, const wchar_t *restrict src, size_t n)
{
	mbstate_t p;

	return wcsrtombs(dest, (void *) &src, n, memset(&p, 0, sizeof(p)));
}
