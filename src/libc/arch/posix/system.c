#include <sys.h>

#include <errno.h>
#include <stdlib.h>

#include "../../syscall.h"

#define SHELL "/bin/sh"

#undef system

extern const char **_environ;

int
system(const char *cmd)
{
	int st;
	pid_t pid;

	if (!cmd)
		return _access(SHELL, X_OK);

	switch ((pid = _fork())) {
	case -1:
		return -1;
	case 0:
		_execve(SHELL, (const char*[]) {"sh", "-c", cmd, NULL}, _environ);
		_exit(127);
	default:
		while (_waitpid(pid, &st, 0) < 0 && errno == EINTR)
			;
		return st;
	}
}
