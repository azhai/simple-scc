#include <errno.h>
#include <stddef.h>

#include "../../libc.h"
#include "../../syscall.h"
#include "brk.h"

int
_brk(void *addr)
{
	if (_sys_brk(addr) != addr) {
		errno = ENOMEM;
		return -1;
	}
	return 0;
}
