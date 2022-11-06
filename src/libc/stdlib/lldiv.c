#include <stdlib.h>

#undef lldiv

lldiv_t lldiv(long long num, long long den)
{
	return (lldiv_t) {num/den, num%den};
}
