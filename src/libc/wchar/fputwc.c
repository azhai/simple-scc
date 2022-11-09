#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <wchar.h>

#undef fputwc

wint_t
_fputwc(wchar_t wc, FILE *fp, int *np)
{
	int n;
	char buf[MB_LEN_MAX];

	if ((n = wcrtomb(buf, wc, NULL)) == -1)
		goto err;
	if (fwrite(buf, 1, n, fp) < n)
		goto err;

	if (np)
		*np = n;

	return wc;

err:
	fp->flags |= _IOERR;
	return WEOF;
}

wint_t
fputwc(wchar_t wc, FILE *fp)
{
	return _fputwc(wc, fp, NULL);
}
