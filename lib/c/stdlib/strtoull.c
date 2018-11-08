#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>

#undef strtoull

unsigned long long
strtoull(const char *s, char **end, int base)
{
	int d, sign = 1;
	unsigned long long n;
	static char digits[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	const char *t, *p;

	while (isspace(*s))
		++s;

	switch (*s) {
	case '-':
		sign = -1;
	case '+':
		++s;
	}

	if (base == 0) {
		if (*s == '0' && toupper(s[1]) == 'X')
			base = 16;
		else if (*s == '0')
			base = 8;
		else
			base = 10;
	}

	if (base == 16 && *s == '0' && toupper(s[1]) == 'X')
		s += 2;

	n = 0;
	for (t = s; p = strchr(digits, toupper(*t)); ++t) {
		if ((d = p - digits) >= base)
			break;
		if (n > ULLONG_MAX/base)
			goto overflow;
		n *= base;
		if (d > ULLONG_MAX - n)
			goto overflow;
		n += d;
	}


	if (end)
		*end = t;
	if (n == 0 && s == t)
		errno = EINVAL;
	return n*sign;

overflow:
	if (end)
		*end = t;
	errno = ERANGE;

	return ULLONG_MAX;
}
