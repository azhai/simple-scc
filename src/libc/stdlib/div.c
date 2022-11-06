#include <stdlib.h>

#undef div

div_t
div(int num, int den)
{
	return (div_t) {num/den, num%den};
}
