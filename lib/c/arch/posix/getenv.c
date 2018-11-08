#include <stdlib.h>
#include <string.h>
#undef getenv

extern char **_environ;

char *
getenv(const char *name)
{
	char **p;
	size_t len = strlen(name);

	for (p = _environ; *p; ++p) {
		if (!memcmp(name, *p, len) && (*p)[len] == '=')
			break;
	}

	if (!*p) 
		return NULL;

	return &(*p)[len];
}
