#include <stdio.h>

void
fn(int a, int b)
{
}

int
main(void)
{
	fn("a", 1);
	fn(1, "a");
	fn(1, 2, 3);

	return 0;
}
