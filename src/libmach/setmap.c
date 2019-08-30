#include <stdio.h>
#include <string.h>

#include <scc/mach.h>

#include "libmach.h"

int
setmap(Map *map, char *name, long off)
{
	int n;
	struct mapsec *sec;

	n = map->n;
	for (sec = map->sec; n--; sec++) {
		if (sec->name && strcmp(sec->name, name) == 0)
			goto found;
	}

	n = map->n;
	for (sec = map->sec; n--; sec++) {
		if (!sec->name)
			goto found;
	}

	/* TODO: Set some errno here */
	return -1;

found:
	sec->name = name;
	sec->offset = off;
	return 0;
}