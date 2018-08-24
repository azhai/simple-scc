
#include <stddef.h>
#include <signal.h>
#include "../../syscall.h"

#undef raise

/* TODO: Add sys.h and store these definitions there */

extern int _getpid(void);
extern int _kill(int pid, int signum);

int
raise(int signum)
{
	return _kill(_getpid(), signum);
}
