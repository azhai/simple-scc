#include <string.h>
#include <time.h>

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

static int
first(int day, int year)
{
	int ny;

	ny = _newyear(year);
	if (ny == day)
		return 0;
	if (day - ny < 0)
		return 7 - (ny - day);
	return day - ny;
}

static int
weeknum(struct tm *tm, int day)
{
	int fday, val;

	fday = first(day, tm->tm_year);
	if (tm->tm_yday < fday) {
		val = 0;
	} else {
		val = tm->tm_yday - fday;
		val /= 7;
		val++;
	}
	return val;
}

static int
isoyear(struct tm *tm)
{
	int monday;

	if (tm->tm_yday < 7) {
		monday = first(THU, tm->tm_year) - 3;
		if (tm->tm_yday < monday)
			return tm->tm_year - 1;
	} else if (tm->tm_yday > 357) {
		monday = first(THU, tm->tm_year + 1) - 3;
		if (tm->tm_mday >= (31 + monday))
			return tm->tm_year + 1;
	}
	return tm->tm_year;
}

static int
isoweek(struct tm *tm)
{
	int year, monday, yday, val;

	year = isoyear(tm);
	monday = first(THU, year) - 3;
	yday = tm->tm_yday;
	if (year > tm->tm_year)
		yday = tm->tm_mday - 31 + monday;
	else if (year < tm->tm_year)
		yday = _daysyear(year) + yday;
	val = yday - monday;
	val /= 7;
	val++;
	return val;
}

static int
isoday(struct tm *tm)
{
	if (tm->tm_wday == 0)
		return 7;
	return tm->tm_wday;
}

static size_t
sval(char *s, size_t siz, char **strs, int abrev, int idx, int max)
{
	char *str;
	size_t len;

	if (siz == 0)
		return 0;

	if (idx < 0 && idx >= max) {
		*s = '?';
		return 1;
	}

	str = strs[idx];
	len = (!abrev) ? strlen(str) : 3;
	if (len > siz)
		len = siz;

	memcpy(s, str, len);
	return len;
}

static size_t
dval(char *s, size_t siz, int prec, int fill, int val)
{
	char *t;
	int n;
	static char digits[] = "0123456789";

	if (siz == 0)
		return 0;

	if (prec > siz)
		prec = siz;

	if (val < 0) {
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
timezone(char *s, size_t siz, const struct tm * restrict tm)
{
	size_t n;
	long off = tm->tm_gmtoff;

	if (off >= 0) {
		*s++ = '+';
	} else {
		*s++ = '-';
		off = -off;
	}

	n = 1;
	n += dval(s, siz-n, 2, '0', off / 3600);
	n += dval(s + n, siz-n, 2, '0', (off % 3600) / 60);

	return n;
}

size_t
strftime(char *restrict s, size_t maxsize,
	 const char *restrict format, const struct tm *restrict timeptr)
{
	int ch, abrev, val, fill, width;
	size_t n, inc;
	char *tfmt;

	for (n = --maxsize; (ch = *format++) && n > 0; s += inc, n -= inc) {
		if (ch != '%') {
			*s = ch;
			inc = 1;
			continue;
		}

		abrev = 0;
		fill = '0';
		width = 2;

		if (*format == 'E' || *format == 'O')
			format++;

		switch (*format++) {
		case 'a':
			abrev = 1;
		case 'A':
			inc = sval(s, n, days, abrev, timeptr->tm_wday, 7);
			break;
		case 'h':
		case 'b':
			abrev = 1;
		case 'B':
			inc = sval(s, n, months, abrev, timeptr->tm_mon, 12);
			break;
		case 'c':
			tfmt = "%a %b %e %T %Y";
			goto recursive;
		case 'C':
			val = (timeptr->tm_year + 1900) / 100;
			goto number;
		case 'd':
			val = timeptr->tm_mday;
			goto number;
		case 'D':
			tfmt = "%m/%d/%y";
			goto recursive;
		case 'e':
			fill = ' ';
			val = timeptr->tm_mday;
			goto number;
		case 'F':
			tfmt = "%Y-%m-%d";
			goto recursive;
		case 'g':
			val = isoyear(timeptr);
			goto number;
		case 'G':
			val = isoyear(timeptr);
			val += 1900;
			width = 4;
			goto number;
		case 'H':
			val = timeptr->tm_hour;
			goto number;
		case 'I':
			val = timeptr->tm_hour;
			if (val == 0)
				val = 12;
			if (val > 12)
				val -= 12;
			goto number;
		case 'j':
			width = 3;
			val = timeptr->tm_yday+1;
			goto number;
		case 'm':
			val = timeptr->tm_mon+1;
			goto number;
		case 'M':
			val = timeptr->tm_min;
			goto number;
		case 'n':
			val = '\n';
			goto character;
		case 'p':
			inc = sval(s, n, am_pm, 0, timeptr->tm_hour > 12, 2);
			break;
		case 'r':
			tfmt = "%I:%M:%S %p";
			goto recursive;
		case 'R':
			tfmt = "%H:%M";
			goto recursive;
		case 'S':
			val = timeptr->tm_sec;
			goto number;
		case 't':
			val = '\t';
			goto character;
		case 'u':
			width = 1;
			val = isoday(timeptr);
			goto number;
		case 'U':
			val = weeknum(timeptr, SUN);
			goto number;
		case 'V':
			val = isoweek(timeptr);
			goto number;
		case 'w':
			width = 1;
			val = timeptr->tm_wday;
			goto number;
		case 'W':
			val = weeknum(timeptr, MON);
			goto number;
		case 'X':
		case 'T':
			tfmt = "%H:%M:%S";
			goto recursive;
		case 'x':
			tfmt = "%m/%d/%y";
			goto recursive;
		case 'y':
			val = timeptr->tm_year%100;
			goto number;
		case 'Y':
			width = 4;
			val = 1900 + timeptr->tm_year;
			goto number;
		case 'z':
			inc = timezone(s, n, timeptr);
			break;
		case 'Z':
			inc = strlen(timeptr->tm_zone);
			if (inc > n)
				inc = n;
			memcpy(s, timeptr->tm_zone, inc);
			break;
		case '\0':
			inc = 0;
			--format;
			break;
		case '%':
			val = '%';
		character:
			*s = val;
			inc = 1;
			break;
		number:
			inc = dval(s, n, width, fill, val);
			break;
		recursive:
			inc = strftime(s, n+1, tfmt, timeptr);
			break;
		}
	}
	*s = '\0';

	return maxsize - n;
}
