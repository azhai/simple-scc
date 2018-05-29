
#include <errno.h>
#include <stdio.h>
#include "syscall.h"

int
_flsbuf(FILE *fp)
{
	int lnbuf = fp->flags & _IOLBF;
	size_t cnt;

	cnt = ((lnbuf) ? fp->lp : fp->wp) - fp->buf;

	if (_write(fp->fd, fp->buf, cnt) != cnt) {
		fp->flags |= _IOERR;
		return EOF;
	}
	fp->rp = fp->wp = fp->buf;

	return 0;
}
