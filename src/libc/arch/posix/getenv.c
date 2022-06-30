#include <stdlib.h>
#include <string.h>

#undef getenv

extern const char **_environ;

char *
getenv(const char *name)
{
	const char **p, *s;
	size_t len = strlen(name);

	for (p = _environ; s = *p; ++p) {
		if (!strncmp(name, s, len) && s[len] == '=')
			return (char *) s + len + 1;
	}
	return NULL;
}
