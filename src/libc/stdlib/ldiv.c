#include <stdlib.h>

#undef ldiv

ldiv_t
ldiv(long num, long den)
{
	return (ldiv_t) {num/den, num%den};
}
