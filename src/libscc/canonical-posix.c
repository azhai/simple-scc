#include <string.h>

#include <scc/scc.h>

char *
canonical(char *path)
{
	char *name = strrchr(path, '/');
	return (name && name[1]) ? name+1 : path;
}
