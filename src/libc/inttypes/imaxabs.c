#include <inttypes.h>

#undef imaxabs

intmax_t
imaxabs(intmax_t n)
{
	return (n < 0) ? -n : n;
}
