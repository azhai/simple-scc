#include <stdlib.h>
#include <errno.h>

#include "../libc.h"

#undef atexit

static void (*funs[_ATEXIT_MAX])(void);
static unsigned nfuns;

static void
callhdls(void)
{
	while (nfuns > 0)
		(*funs[--nfuns])();
}

int
atexit(void (*fun)(void))
{
	if (nfuns == _ATEXIT_MAX) {
		errno = ENOMEM;
		return -1;
	}
	funs[nfuns++] = fun;
	_atexithdl = callhdls;

	return 0;
}
