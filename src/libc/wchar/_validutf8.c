#include <wchar.h>

#include "../libc.h"

struct range {
	unsigned long begin, end;
	int valid;
	int nbytes;
};

int
_validutf8(wchar_t wc, int *nbytes)
{

	static struct range ranges[] = {
		{0,         0x80,       1, 1},
		{0x80,      0x800,      1, 2},
		{0x800,     0xD800,     1, 3},
		{0xD800,    0xDD00,     0, 3},
		{0xDD00,    0x10000,    1, 3},
		{0x10000,   0x110000,   1, 4},
		{0x11000,   -1ul,       0, 0},
	};
	struct range *bp;

	for (bp = ranges; bp->begin <= wc && bp->end > wc; ++bp)
		;
	*nbytes = bp->nbytes;

	return bp->valid;
}
