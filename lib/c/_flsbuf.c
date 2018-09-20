#include <errno.h>
#include <stdio.h>
#include "syscall.h"

int
_flsbuf(FILE *fp)
{
	int lnbuf = fp->flags & _IOLBF;
	unsigned char *p;
	size_t cnt;

	p = (lnbuf) ? fp->lp : fp->wp;
	cnt = p - fp->buf;

	if (_write(fp->fd, fp->buf, cnt) != cnt) {
		fp->flags |= _IOERR;
		return EOF;
	}
	fp->lp = fp->rp = fp->wp = fp->buf;

	return 0;
}
