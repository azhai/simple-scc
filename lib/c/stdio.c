#include <stdio.h>

FILE __iob[FOPEN_MAX] = {
	{
		.fd = 0,
		.buf = NULL,
		.len = BUFSIZ,
		.flags = _IOREAD,
		.lp = inbuf,
		.rp = inbuf,
		.wp = inbuf,
	},
	{
		.fd = 1,
		.buf = NULL,
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
