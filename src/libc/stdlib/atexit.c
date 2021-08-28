#include <stdlib.h>
#include <errno.h>

#include "../libc.h"

#undef atexit

int
atexit(void (*fun)(void))
{
	if (_exitn == _ATEXIT_MAX) {
		errno = ENOMEM;
		return -1;
	}

	_exitf[_exitn++] = fun;

	return 0;
}
