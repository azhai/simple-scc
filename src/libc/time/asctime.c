#include <time.h>

#undef asctime

char *
asctime(const struct tm *tm)
{
	static char buf[26];

	/*
	 * buf has space for a text like "Sun Sep 16 01:03:52 1973\n\0"
	 * and for this reason strftime cannot fail as the buffer is
	 * long enough
	 */
	strftime(buf, sizeof(buf), "%c\n", tm);
	return buf;
}
