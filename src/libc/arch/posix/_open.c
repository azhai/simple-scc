#include <stddef.h>

#include <sys.h>

#include "../../syscall.h"

extern int _openat(int, const char *, int, int);

int
_open(const char *fname, int flags, int mode)
{
	return _openat(AT_FDCWD, fname, flags, mode);
}
