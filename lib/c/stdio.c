#include <stdio.h>

static unsigned char inbuf[BUFSIZ];
static unsigned char outbuf[BUFSIZ];

FILE __iob[FOPEN_MAX] = {
	{
		.fd = 0,
		.buf = inbuf,
		.len = BUFSIZ,
		.flags = _IOREAD,
		.lp = inbuf,
		.rp = inbuf,
		.wp = inbuf,
	},
	{
		.fd = 1,
		.buf = outbuf,
		.len = BUFSIZ,
		.flags = _IOWRITE | _IOLBF,
		.lp = outbuf,
		.rp = outbuf,
		.wp = outbuf,
	},
	{
		.fd = 2,
		.buf = stderr->unbuf,
		.len = sizeof(stderr->unbuf),
		.flags = _IOWRITE | _IONBF,
		.rp = stderr->unbuf,
		.wp = stderr->unbuf,
	},
};
