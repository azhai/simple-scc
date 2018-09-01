#include <assert.h>
#include <stdio.h>

int
main()
{
	int i;
	char c;

	printf("First assert\n");
	assert(sizeof(i) >= sizeof(c));

#define NDEBUG
#include <assert.h>

	printf("Second assert, that must fail\n");
	assert(sizeof(i) < sizeof(c));

	return 0;
}
