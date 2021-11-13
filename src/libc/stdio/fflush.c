#include <stdio.h>

#include "../libc.h"

#undef fflush

int
fflush(FILE *fp)
{
	int err;

	if (fp)
		return _flsbuf(fp);

	err = 0;
	for (fp = __iob; fp < &__iob[FOPEN_MAX]; ++fp) {
		if ((fp->flags & _IOWRITE) != 0 && _flsbuf(fp))
			err = EOF;
	}
	return err;
}
