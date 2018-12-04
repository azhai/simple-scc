static char sccsid[] = "@(#) ./lib/scc/xmalloc.c";
#include <stdlib.h>
#include <scc/scc.h>

void *
xmalloc(size_t size)
{
	void *p = malloc(size);

	if (!p)
		die("out of memory");
	return p;
}
