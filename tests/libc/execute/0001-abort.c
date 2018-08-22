
#include <signal.h>
#include <stdlib.h>

void
handler(int dummy)
{
	exit(EXIT_SUCCESS);
}

int
main(void)
{
	signal(SIGABRT, handler);
	abort();

	return 1;
}
