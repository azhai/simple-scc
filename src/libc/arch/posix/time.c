#include <time.h>

int
_gettimeofday(struct timeval *restrict, void *tzp);

time_t
time(time_t *t)
{
	struct timeval tv;

	if (_gettimeofday(&tv, NULL) == -1)
		return -1;
	if (t)
		*t =tv.tv_sec;
	return tv.tv_sec;
}
