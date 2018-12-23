#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>

#undef strtoul

unsigned long
strtoul(const char *s, char **end, int base)
{
	int d, sign = 1;
	unsigned long n;
	static const char digits[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	const char *t, *p;

	if (end)
		*end = s;

	while (isspace(*s))
		++s;

	switch (*s) {
	case '-':
		sign = -1;
	case '+':
		++s;
	}

	if (base == 0) {
		if (*s == '0')
			base = toupper(s[1]) == 'X' ? 16 : 8;
		else
			base = 10;
	}
	if (base == 16 && *s == '0' && toupper(s[1]) == 'X')
		s += 2;

	n = 0;
	for (t = s; p = strchr(digits, toupper(*t)); ++t) {
		if ((d = p - digits) >= base)
			break;
		if (n > ULONG_MAX/base)
			goto overflow;
		n *= base;
		if (d > ULONG_MAX - n)
			goto overflow;
		n += d;
	}

	if (end && t != s)
		*end = t;

	return n*sign;

overflow:
	if (end)
		*end = t;
	errno = ERANGE;

	return ULONG_MAX;
}
