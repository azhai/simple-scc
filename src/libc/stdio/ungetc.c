#include <stdio.h>

/**
 * ungetc() - push back one character to a input stream
 *
 * Context: A stream just opened has rp, wp, lp and buf pointing to NULL,
 *          in the same way that a closed stream. In both cases we have to
 *          return EOF, so the check fp->rp == fp->buf detects both cases.
 *          An input stream can be either a read only input or a read and
 *          write input in read state, and we can detect the write state
 *          when wp does not point to the beginning of buf. _IOSTRG is used
 *          in sprintf/sscanf functions, where it is possible rp points to
 *          a constant string, so we cannot write back c, but it is safe
 *          because in those functions we are going to push back always
 *          the same character that the one contained in the string.
 */
int
ungetc(int c, FILE *fp)
{
	if (c == EOF)
		return EOF;

	if ((fp->flags & _IOWRITE) != 0)
		return EOF;

	if (fp->rp == fp->buf || fp->wp != fp->buf)
		return EOF;

	--fp->rp;
	if ((fp->flags & _IOSTRG) == 0)
		*fp->rp = c;
	fp->flags &= ~_IOEOF;

	return c;
}
