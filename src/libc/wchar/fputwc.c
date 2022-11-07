#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <wchar.h>

#undef fputwc

wint_t
_fputwc(wchar_t wc, FILE *fp, int *np)
{
	int n;
	mbstate_t state;
	char buf[MB_LEN_MAX];

	memset(&state, 0, sizeof(state));
	if ((n = wcrtomb(buf, wc, &state)) == -1)
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
