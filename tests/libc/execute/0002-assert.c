
#include <assert.h>
#include <signal.h>

void
handler(void)
{
	_Exit(0);
}

int
main()
{
	int i;
	char c;

	signal(SIGABRT, handler);

	printf("First assert\n");
	assert(sizeof(i) >= sizeof(c));

	printf("Second assert, that must fail\n");
	assert(sizeof(i) < sizeof(c));

	return 1;
}
