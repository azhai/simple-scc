#include <time.h>

#undef difftime

double
difftime(time_t t1, time_t t2)
{
	return t2 - t1;
}
