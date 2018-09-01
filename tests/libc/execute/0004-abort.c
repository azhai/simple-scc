#include <assert.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

int
main(void)
{
	printf("aborting\n");
	assert(signal(SIGABRT, SIG_IGN) != SIG_ERR);
	abort();
	printf("borning\n");

	return 0;
}
