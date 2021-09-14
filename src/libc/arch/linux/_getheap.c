#include <stddef.h>

#include "../../libc.h"
#include "../../syscall.h"
#include "brk.h"

void *
_getheap(void)
{
	return _sys_brk(0);
}
