#include <stdio.h>
#include <string.h>

#include <scc/mach.h>

#include "libmach.h"

int
setmap(Map *map,
       char *name,
       FILE *fp,
       unsigned long long begin,
       unsigned long long end,
       long off)
{
	int n;
	Mapsec *sec;

	n = map->n;
	for (sec = map->sec; n--; sec++) {
		if (!sec->name || !strcmp(sec->name, name))
			goto found;
	}

	if ((map = remap(map, map->n+1 )) == NULL)
		return -1;
	sec = &map->sec[map->n-1];

found:
	sec->name = name;
	sec->fp = fp,
	sec->begin = begin;
	sec->end = end;
	sec->offset = off;
	return 0;
}
