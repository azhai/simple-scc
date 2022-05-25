#include <stdlib.h>

#include "../libc.h"
#include "../syscall.h"

#undef exit

void (*_flushall)(void);
void (*_atexithdl)(void);

void
exit(int status)
{
	if (_atexithdl)
		(*_atexithdl)();
	if (_flushall)
		(*_flushall)();
	_exit(status);
}
