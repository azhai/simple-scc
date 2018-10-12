#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

extern int _flsbuf(FILE *fp);

int
fflush(FILE *fp)
{
	int err;

	if (fp)
		return _flsbuf(fp);

	err = 0;
	for (fp = __iob; fp < &__iob[FOPEN_MAX]; ++fp) {
		if ((fp->flags & _IOWRITE) == 0 && _flsbuf(fp))
			err = EOF;
	}
	return err;
}

static void
cleanup(void)
{
	fflush(NULL);
}

int
__putc(int ch, FILE *fp)
{
	static int first = 1;

	if (fp->flags & _IOERR)
		return EOF;

	if (fp->flags & _IOREAD) {
		fp->flags |= _IOERR;
		errno = EBADF;
		return EOF;
	}

	if (fp->flags & _IOSTRG) {
		fp->flags |= _IOERR;
		return EOF;
	}

	if (first) {
		if (atexit(cleanup)) {
			fp->flags |= _IOERR;
			errno = ENOMEM;
			return EOF;
		}
		first = 0;
	}

	if (fp->flags & _IOLBF) {
		if (fp->lp == fp->rp && _flsbuf(fp))
			return EOF;
		*fp->lp++ = ch;
		if (ch == '\n' && _flsbuf(fp))
			return EOF;
	} else if (fp->flags & _IOFBF) {
		if (fp->wp == fp->rp && _flsbuf(fp))
			return EOF;
		*fp->wp++ = ch;
	} else {
		*fp->wp++ = ch;
		if (_flsbuf(fp))
			return EOF;
	}

	fp->flags |= _IOWRITE;
	return ch & 0xFF;
}
