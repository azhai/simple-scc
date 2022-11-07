#include <wchar.h>

#undef wcslen

size_t
wcslen(const wchar_t *ws)
{
	const wchar_t *p;

	for (p = ws; *p != L'\0'; p++)
		;
	return p - ws;
}
