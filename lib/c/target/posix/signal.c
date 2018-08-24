
#include <signal.h>

#undef signal

void
(*signal(int signum, void (*func)(int)))(int)
{
}
