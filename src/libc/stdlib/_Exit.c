#include <stdlib.h>

#include "../syscall.h"

#undef _Exit

void
_Exit(int status)
{
	_exit(status);
}
