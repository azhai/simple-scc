#include <inttypes.h>

#undef imaxdiv

imaxdiv_t
imaxdiv(intmax_t num, intmax_t den)
{
	return (imaxdiv_t) {num/den, num%den};
}
