#include <stdio.h>

#include <scc/mach.h>

#include "libmach.h"

static int (*freadv[])(Obj *, FILE *) = {
	[COFF32] = coff32read,
};

static int (*fsymsv[])(Obj *) = {
	[COFF32] = coff32getsyms,
};

static int (*fsectsv[])(Obj *) = {
	[COFF32] = coff32getsect,
};

static int
getsects(Obj *obj)
{
	int fmt;

	fmt = FORMAT(obj->type);
	if (fmt >= NFORMATS)
		return -1;

	return  (*fsectsv[fmt])(obj);
}

static int
getsyms(Obj *obj)
{
	int fmt;

	fmt = FORMAT(obj->type);
	if (fmt >= NFORMATS)
		return -1;

	return  (*fsymsv[fmt])(obj);
}

static int
readfile(Obj *obj, FILE *fp)
{
	int fmt;

	fmt = FORMAT(obj->type);
	if (fmt >= NFORMATS)
		return -1;

	if ((*freadv[fmt])(obj, fp) < 0)
		return -1;
	obj->fp = fp;

	return 0;
}

int
objread(Obj *obj, FILE *fp)
{
	objfree(obj, TARGETDEL | GENERICDEL);

	if (readfile(obj, fp) < 0)
		goto err;
	if (getsyms(obj) < 0)
		goto err;
	if (getsects(obj) < 0)
		goto err;
	return 0;

err:
	objfree(obj, TARGETDEL | GENERICDEL);
	return -1;
}
