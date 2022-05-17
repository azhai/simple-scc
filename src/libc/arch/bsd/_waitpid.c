#include <stddef.h>

#include <sys.h>

int
_waitpid(pid_t pid, int *status, int options)
{
	return _wait4(pid, status, options, NULL);
}
