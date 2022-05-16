#include <time.h>

#include "../libc.h"

#undef gmtime

struct tm *
gmtime(const time_t *tim)
{
	static struct tm tm;
	time_t day;
	int i;

	tm.tm_sec = *tim % SECDAY;
	tm.tm_min = tm.tm_sec / 60;
	tm.tm_sec %= 60;
	tm.tm_hour = tm.tm_min / 60;
	tm.tm_min %= 60;
	day = *tim / SECDAY;

	tm.tm_wday = (day + THU) % 7; /* 1/1/1970 was Thursday */

	for (i = EPOCH; day >= _daysyear(i); ++i)
		day -= _daysyear(i);
	tm.tm_year = i - 1900;
	tm.tm_yday = day;

	_daysmon[FEB] = FEBDAYS(i);
	for (i = JAN; day >= _daysmon[i]; i++)
		day -= _daysmon[i];
	tm.tm_mon = i;
	tm.tm_mday = day + 1;

	tm.tm_isdst = 0;
	tm.tm_zone = "GMT";
	tm.tm_gmtoff = 0;

	return &tm;
}
