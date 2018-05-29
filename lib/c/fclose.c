
#include <stdio.h>
#undef fclose

extern int _flsbuf(FILE *fp);

int
fclose(FILE *fp)
{
	int r = EOF;

	if ((fp->flags & _IOSTRG) == 0 &&
	    fp->flags & (_IOWRITE | _IOREAD | _IOWR)) {
		r = 0;
		if (_flsbuf(fp) == EOF)
			r = EOF;
		if (close(fp->fd) < 0)
			r = EOF;
	}

	if (fp->flags & _IOALLOC) {
		free(fp->buf);
		fp->buf = NULL;
	}

	fp->flags &= ~(_IOWRITE | _IOREAD | _IOWR |
	               _IOERR | _IOEOF |
	               _IOALLOC |
	               _IOTXT |
	               _IOSTRG);

	return r;
}
