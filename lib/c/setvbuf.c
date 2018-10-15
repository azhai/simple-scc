#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#undef setvbuf

extern int _flsbuf(FILE *fp);

int
setvbuf(FILE * restrict fp, char * restrict buf, int mode, size_t size)
{
	int flags;

	if (_flsbuf(fp) == EOF)
		return EOF;

	switch (mode) {
	case _IONBF:
		size = sizeof(fp->unbuf);
		buf = fp->unbuf;
		break;
	case _IOLBF:
	case _IOFBF:
		if (size == 0) {
			if ((buf = malloc(BUFSIZ)) == NULL) {
				errno = ENOMEM;
				return EOF;
			}
			size = BUFSIZ;
		}
		break;
	default:
		errno = EINVAL;
		return EOF;
	}

	flags = fp->flags;
	if (flags & _IOALLOC)
		free(fp->buf);
	flags &= ~(_IONBF | _IOLBF | _IOFBF | _IOALLOC | _IOALLOC);
	flags |= mode;
	fp->flags = flags;
	fp->buf = buf;
	fp->len = size;

	return 0;
}
