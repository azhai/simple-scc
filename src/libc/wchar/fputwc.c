#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <wchar.h>

#undef fputwc

wint_t
fputwc(wchar_t wc, FILE *fp)
{
	int n;
	mbstate_t state;
	char buf[MB_LEN_MAX];

	memset(&state, 0, sizeof(state));
	if ((n = wcrtomb(buf, wc, &state)) == -1)
		goto err;
	if (fwrite(buf, 1, n, fp) < n)
		goto err;
	return wc;

err:
	fp->flags |= _IOERR;
	return WEOF;
}
