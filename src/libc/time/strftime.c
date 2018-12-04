#include <time.h>
#include <string.h>

#include "../libc.h"
#undef strftime

static char *days[] = {
	"Sunday",   "Monday", "Tuesday",  "Wednesday",
	"Thursday", "Friday", "Saturday", 
};

static char *months[] = {
	"January",   "February", "March",    "April",
	"May",       "June",     "July",     "August",
	"September", "October",  "November", "December"
};

static char *am_pm[] = {"AM", "PM"};

static size_t
sval(char *s, size_t siz, char **strs, int abrev, int idx, int max)
{
	char *str;
	size_t len;

	if (idx < 0 && idx >= max)
		goto wrong;

	str = strs[idx];
	len = (!abrev) ? strlen(str) : 3;
	if (len > siz)
		goto wrong;

	memcpy(s, str, len);
	return len;

wrong:
	*s = '?';
	return 1;
}

static size_t
dval(char *s, size_t siz, int prec, int fill, int val)
{
	char *t;
	int n;
	static char digits[] = "0123456789";

	if (prec > siz || val < 0) {
		*s = '?';
		return 1;
	}

	n = prec;
	do {
		s[--n] = digits[val % 10];
		val /= 10;
	} while (n > 0 && val > 0);

	while (n > 0)
		s[--n] = fill;

	return prec;
}

static size_t
timezone(char *s, size_t prec, const struct tm * restrict tm)
{
	long off = tm->tm_gmtoff;

	if (prec < 5) {
		*s = '?';
		return 1;
	}

	if (off >= 0) {
		*s++ = '+';
	} else {
		*s++ = '-';
		off = -off;
	}

	dval(s, 2, 2, '0', off / 3600);
	dval(s, 2, 2, '0', (off % 3600) / 60);

	return 5;
}

size_t
strftime(char * restrict s, size_t siz,
         const char * restrict fmt,
         const struct tm * restrict tm)
{
	int ch, abrev, val, fill, width;
	size_t n, inc;
	char *tfmt;

	for (n = siz-1; (ch = *fmt++) && n > 0; s += inc, n -= inc) {
		if (ch != '%') {
			*s = ch;
			inc = 1;
			continue;
		}

		abrev = 0;
		fill = '0';
		width = 2;

		switch (*fmt++) {
		case 'Z':
			if (!tm->tm_zone)
				break;
			inc = sval(s, n, &tm->tm_zone, 0, 0, 1);
			break;
		case 'a':
			abrev = 1;
		case 'A':
			inc = sval(s, n, days, abrev, tm->tm_wday, 7);
			break;
		case 'h':
		case 'b':
			abrev = 1;
		case 'B':
			inc = sval(s, n, months, abrev, tm->tm_mon, 12);
			break;
		case 'p':
			inc = sval(s, n, am_pm, 0, tm->tm_hour > 12, 2);
			break;
		case 'c':
			tfmt = "%a %b %e %T %Y";
			goto recursive;
		case 'D':
			tfmt = "%m/%d/%y";
			goto recursive;
		case 'F':
			tfmt = "%Y-%m-%d";
			goto recursive;
		case 'R':
			tfmt = "%H:%M";
			goto recursive;
		case 'X':
		case 'T':
			tfmt = "%H:%M:%S";
			goto recursive;
		case 'r':
			tfmt = "%I:%M:%S %p";
			goto recursive;
		case 'x':
			tfmt = "%m/%d/%y";
			goto recursive;
		recursive:
			inc = strftime(s, n+1, tfmt, tm) - 1;
			break;
		case 'n':
			val = '\n';
			goto character;
		case 't': 
			val = '\t';
			goto character;
		case '%': 
			val = '%';
		character:
			*s = val;
			inc = 1;
			break;
		case 'e':
			fill = ' ';
			val = tm->tm_mday;
			goto number;
		case 'd':
			val = tm->tm_mday;
			goto number;
		case 'V':
		case 'g':
		case 'G':
			/* TODO */
			break;
		case 'C':
			val = tm->tm_year / 100;
			goto number;
		case 'H':
			val = tm->tm_hour;
			goto number;
		case 'I':
			val = tm->tm_hour;
			if (val == 0)
				val = 12;
			if (val > 12)
				val -= 12;
			goto number;
		case 'j':
			width = 3;
			val = tm->tm_yday+1;
			goto number;
		case 'm':
			val = tm->tm_mon+1;
			goto number;
		case 'M':
			val = tm->tm_min;
			goto number;
		case 'S':
			val = tm->tm_sec;
			goto number;
		case 'u':
			width = 1;
			val = tm->tm_wday+1;
			goto number;
		case 'U':
			val = tm->tm_yday / 7;
			if (_newyear(tm->tm_year) == SAT)
				val++;
			goto number;
		case 'W':
			val = tm->tm_yday / 7;
			if (_newyear(tm->tm_year) == MON)
				val++;
			goto number;
		case 'w':
			width = 1;
			val = tm->tm_wday;
			goto number;
		case 'y':
			val = tm->tm_year%100;
			goto number;
		case 'Y':
			width = 4;
			val = 1900 + tm->tm_year;
		number:
			inc = dval(s, n, width, fill, val);
			break;
		case 'z':
			inc = timezone(s, n, tm);
			break;
		case 'E':
		case 'O':
			if (*fmt != '\0')
				fmt += 2;;
		case '\0':
			inc = 0;
			--fmt;
			break;
		}
	}
	*s = '\0';

	return siz - n;
}
