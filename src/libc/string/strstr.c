#include <stddef.h>
#include <string.h>

#undef strstr

char *
strstr(const char *s1, const char *s2)
{
	const char *p;
	int c = *s2;
	int len;

	if ((len = strlen(s2)) == 0)
		return s1;

	for (p = s1; p = strchr(p, c); ++p) {
		if (!strncmp(p, s2, len))
			return p;
	}

	return NULL;
}
