#include <signal.h>

#include <sys.h>

#undef signal

void
(*signal(int signum, void (*func)(int)))(int)
{
	struct sigaction osa, sa = {
		.sa_handler = func,
	};

	if (_sigaction(signum, &sa, &osa) < 0)
		return SIG_ERR;

	return osa.sa_handler;
}
