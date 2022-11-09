#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#undef mbstowcs

size_t
mbstowcs(wchar_t *restrict dest, const char *restrict src, size_t n)
{
	mbstate_t p;

	return mbsrtowcs(dest, (void *) &src, n, memset(&p, 0, sizeof(p)));
}
