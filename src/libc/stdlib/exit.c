#include <stdlib.h>

#include "../libc.h"
#include "../syscall.h"

#undef exit

void (*_exitf[_ATEXIT_MAX])(void);
unsigned _exitn;
void (*_flushall)(void);

void
exit(int status)
{
	while (_exitn > 0)
		(*_exitf[--_exitn])();
	if (_flushall)
		(*_flushall)();
	_exit(status);
}
