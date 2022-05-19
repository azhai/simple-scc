#include <stdio.h>

#include "../syscall.h"

#undef rename

int
rename(const char *old, const char *new)
{
	return _rename(old, new);
}
