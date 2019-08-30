#include <errno.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <scc/mach.h>

#include "libmach.h"

Map *
newmap(int n, FILE *fp)
{
	size_t siz, vsiz;
	struct mapsec *p;
	Map *map;

	siz = sizeof(*map);
	if (n > SIZE_MAX/sizeof(struct mapsec))
		goto out_range;

	vsiz = n * sizeof(struct mapsec);
	if (vsiz > SIZE_MAX - siz)
		goto out_range;

	if ((map = malloc(siz + vsiz)) == NULL)
		return NULL;

	map->n = n;
	memset(map->sec, 0, vsiz);

	for (p = map->sec; n--; ++p)
		p->fp = fp;
	return map;

out_range:
		errno = ERANGE;
		return NULL;
}
