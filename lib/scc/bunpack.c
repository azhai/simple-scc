static char sccsid[] = "@(#) ./lib/scc/lunpack.c";

#include <ctype.h>
#include <stdarg.h>

#include "../../inc/scc.h"

int
lunpack(unsigned char *src, char *fmt, ...)
{
	unsigned char *bp, *cp;
	unsigned short *sp;
	unsigned s;
	unsigned long *lp, l;
	unsigned long long *qp, q;
	va_list va;
	size_t n;
	int d;

	bp = src;
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
				*cp++ = *bp++;
			break;
		case 'c':
			cp = va_arg(va, unsigned char *);
			*cp = *bp++;
			break;
		case 's':
			sp = va_arg(va, unsigned short *);
			s =  (unsigned) *bp++ << 8;
			s |= (unsigned) *bp++;
			*sp = s;
			break;
		case 'l':
			lp = va_arg(va, unsigned long *);
			l =  (unsigned long) *bp++ << 24;
			l |= (unsigned long) *bp++ << 16;
			l |= (unsigned long) *bp++ << 8;
			l |= (unsigned long) *bp++;
			*lp = l;
			break;
		case 'q':
			qp = va_arg(va, unsigned long long *);
			q =  (unsigned long long) *bp++ << 56;
			q |= (unsigned long long) *bp++ << 48;
			q |= (unsigned long long) *bp++ << 40;
			q |= (unsigned long long) *bp++ << 32;
			q |= (unsigned long long) *bp++ << 24;
			q |= (unsigned long long) *bp++ << 16;
			q |= (unsigned long long) *bp++ << 8;
			q |= (unsigned long long) *bp++;
			*qp = q;
			break;
		default:
			va_end(va);
			return -1;
		}
	}
	va_end(va);

	return bp - src;
}
