#include <stdarg.h>
#include <stdio.h>

#undef vsnprintf

int
vsnprintf(char *restrict buf, size_t siz, const char *restrict fmt, va_list ap)
{
	FILE f;
	int r;
	unsigned char *s = (unsigned char *) buf;

	f.flags = _IORW | _IOSTRG;
	f.len = siz;
	f.buf = s;
	f.wp = s;
	f.rp = s + siz;

	r = vfprintf(&f, fmt, ap);
	if (s) {
		if (f.wp == f.rp)
			--f.wp;
		if (siz != 0)
			*f.wp = '\0';
	}

	return r;
}
