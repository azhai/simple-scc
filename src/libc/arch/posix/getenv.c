#include <sys.h>

#include <stdlib.h>
#include <string.h>

#undef getenv

char *
getenv(const char *name)
{
	char **p, *s;
	size_t len = strlen(name);

	for (p = _environ; s = *p; ++p) {
		if (!strncmp(name, s, len) && s[len] == '=')
			return s + len + 1;
	}
	return NULL;
}
