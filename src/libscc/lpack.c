#include <ctype.h>
#include <stdarg.h>

#include <scc/scc.h>

int
lpack(unsigned char *dst, char *fmt, ...)
{
	unsigned char *bp, *cp;
	unsigned s;
	unsigned long l;
	unsigned long long q;
	size_t n;
	int d;
	va_list va;

	bp = dst;
	va_start(va, fmt);
	while (*fmt) {
		switch (*fmt++) {
		case '\'':
			for (n = 0; isdigit(*fmt); n += d) {
				n *= 10;
				d = *fmt++ - '0';
			}
			cp = va_arg(va, unsigned char *);
			while (n--)
				*bp++ = *cp++;
			break;
		case 'c':
			*bp++ = va_arg(va, unsigned);
			break;
		case 's':
			s = va_arg(va, unsigned);
			*bp++ = s;
			*bp++ = s >> 8;
			break;
		case 'l':
			l = va_arg(va, unsigned long);
			*bp++ = l;
			*bp++ = l >> 8;
			*bp++ = l >> 16;
			*bp++ = l >> 24;
			break;
		case 'q':
			q = va_arg(va, unsigned long long);
			*bp++ = q;
			*bp++ = q >> 8;
			*bp++ = q >> 16;
			*bp++ = q >> 24;
			*bp++ = q >> 32;
			*bp++ = q >> 40;
			*bp++ = q >> 48;
			*bp++ = q >> 56;
			break;
		default:
			va_end(va);
			return -1;
		}
	}
	va_end(va);

	return bp - dst;
}
