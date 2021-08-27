#include <string.h>

#undef strtok

char *
strtok(char * restrict s1, const char * restrict s2)
{
	static char *line;

	if (s1)
		line = s1;
	else if (!line)
		return NULL;

	s1 = line + strspn(line, s2);
	if (*s1 == '\0')
		return line = NULL;

	line = s1 + strcspn(s1, s2);
	if (*line == '\0')
		line = NULL;
	else
		*line++ = '\0';

	return s1;
}
