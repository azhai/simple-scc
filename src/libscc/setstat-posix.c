#include <sys/stat.h>
#include <unistd.h>
#include <utime.h>

#include <time.h>

#include <scc/scc.h>

int
setstat(char *fname, struct fprop *prop)
{
	struct utimbuf ut = {prop->time, prop->time};

	if (chown(fname, prop->uid, prop->gid) < 0) {
		if (chown(fname, getuid(), getgid()) < 0)
			return -1;
	}
	if (chmod(fname, prop->mode) < 0)
		return -1;
	if (utime(fname, &ut) < 0)
		return -1;
	return 0;
}
