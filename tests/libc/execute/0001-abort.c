#include <assert.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

/*
output:
aborting
end:
*/

int ret = 1;

void
handler(int dummy)
{
	_Exit(ret);
}

int
main(void)
{
	printf("aborting\n");
	assert(signal(SIGABRT, handler) != SIG_ERR);
	ret = 0;
	abort();
	printf("borning\n");

	return 1;
}
