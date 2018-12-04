#include <time.h>

#include "../libc.h"
#undef localtime

struct tm *
localtime(const time_t *timep)
{
	struct tzone *tz;
	struct tm *tm;
	time_t t = *timep;

	t += tz->gmtoff * 60;
	t += tz->isdst * 60;
	tm = gmtime(&t);
	tz = _tzone(tm);
	tm->tm_zone = tz->name;
	tm->tm_isdst = tz->isdst;
	tm->tm_gmtoff = tz->gmtoff;

	return tm;
}
