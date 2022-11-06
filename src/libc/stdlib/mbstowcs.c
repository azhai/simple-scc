#include <stdlib.h>
#include <wchar.h>

#undef mbstowcs

size_t
mbstowcs(wchar_t *restrict dest, const char *restrict src, size_t n)
{
	return mbsrtowcs(dest, (void *) &src, n, NULL);
}
