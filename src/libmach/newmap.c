#include <errno.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <scc/mach.h>

#include "libmach.h"

Map *
remap(Map *map, int n)
{
	size_t vsiz;

	if (n > SIZE_MAX/sizeof(struct mapsec))
		goto out_range;
	vsiz = n * sizeof(struct mapsec);
	if (vsiz > SIZE_MAX - sizeof(*map))
		goto out_range;
	vsiz += sizeof(*map);

	if ((map = realloc(map, vsiz)) == NULL)
		return NULL;

	map->n = n;
	return map;

	return map;

out_range:
	errno = ERANGE;
	return NULL;
}

Map *
newmap(Map *map, int n)
{
	if ((map = remap(map, n)) == NULL)
		return NULL;
	memset(map->sec, 0, n * sizeof(struct mapsec));

	return map;
}
