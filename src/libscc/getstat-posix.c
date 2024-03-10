#include <sys/stat.h>

#include <limits.h>
#include <time.h>

#include <scc/scc.h>

int
getstat(char *fname, struct fprop *prop)
{
	struct stat st;

	if (stat(fname, &st) < 0)
		return -1;
	if (st.st_size > LONG_MAX)
		return -1;
	prop->uid = st.st_uid;
	prop->gid = st.st_gid;
	prop->mode = st.st_mode;
	prop->time = st.st_mtime;
	prop->size = st.st_size;

	return 0;
}
