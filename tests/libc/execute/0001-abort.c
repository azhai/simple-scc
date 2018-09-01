
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

void
handler(int dummy)
{
	_Exit(0);
}

int
main(void)
{
	printf("aborting\n");
	signal(SIGABRT, handler);
	abort();
	printf("borning\n");

	return 1;
}
