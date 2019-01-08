static char sccsid[] = "@(#) ./ar/posix/driver.c";

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "sys.h"

time_t
totime(long long t)
{
	return t;
}

int
getstat(char *fname, struct fprop *prop)
{
	struct stat st;

	if (stat(fname, &st) < 0)
		return -1;
	prop->uid = st.st_uid;
	prop->gid = st.st_gid;
	prop->mode = st.st_mode;
	prop->time = st.st_mtime;

	return 0;
}

int
setstat(char *fname, struct fprop *prop)
{
}
