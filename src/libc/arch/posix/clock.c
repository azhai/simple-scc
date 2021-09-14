#include <time.h>

#include <sys.h>

#include "time.h"

#define TOCLOCK(r) (r.tv_sec * CLOCKS_PER_SEC +\
                    r.tv_usec / (1000000 / CLOCKS_PER_SEC))

struct rusage {
	struct timeval ru_utime;
	struct timeval ru_stime;
	long int ru_maxrss;
	long int ru_ixrss;
	long int ru_idrss;
	long int ru_isrss;
	long int ru_minflt;
	long int ru_majflt;
	long int ru_nswap;
	long int ru_inblock;
	long int ru_oublock;
	long int ru_msgsnd;
	long int ru_msgrcv;
	long int ru_nsignals;
	long int ru_nvcsw;
	long int ru_nivcsw;
};

extern int _getrusage(int, struct rusage*);

clock_t
clock(void)
{
	struct rusage ru;

	if (_getrusage(RUSAGE_SELF, &ru))
		return -1;

	return TOCLOCK(ru.ru_utime) + TOCLOCK(ru.ru_stime);
}
