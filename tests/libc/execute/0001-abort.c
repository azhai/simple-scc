
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

sig_atomic_t v;

void
handler(int dummy)
{
	v = 1;
}

int
main(void)
{
	signal(SIGABRT, handler);
	abort();

	if (v == 1)
		printf("ok\n");

	return 1;
}
