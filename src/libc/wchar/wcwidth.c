#include <wchar.h>
#undef wcwidth

/* incomplete, needs a real implementation */
int
wcwidth(wchar_t wc)
{
	if (!wc)
		return 0;
	else if (wc < 32 || wc >= 127)
		return -1;
	return 1;
}
