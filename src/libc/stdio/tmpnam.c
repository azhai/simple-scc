#include <sys.h>

#include <stdio.h>
#include <string.h>

#include "../syscall.h"

#undef tmpnam

char *
tmpnam(char *s)
{
	static char tmpl[] = _TMPNAME;
	char *p;

	for (;;) {
		for (p = tmpl; *p && *p != '9'; ++p)
			;
		if (*p == '\0')
			return NULL;
		++*p;

		if (_access(tmpl, F_OK) != 0)
			break;
	}
	if (s)
		strcpy(s, tmpl);
	return tmpl;
}
