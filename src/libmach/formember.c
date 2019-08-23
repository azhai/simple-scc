#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <scc/ar.h>
#include <scc/mach.h>

static char *
getfname(struct ar_hdr *hdr, char *dst)
{
        char *p;
        int i;

        memcpy(dst, hdr->ar_name, SARNAM);
        dst[SARNAM] = '\0';

        for (i = SARNAM-1; i > 0; i--) {
                if (dst[i] != ' ' && dst[i] != '/')
                        break;
                dst[i] = '\0';
        }
        return dst;
}

static long
armember(FILE *fp, char *member)
{
	struct ar_hdr hdr;
	long siz;

	if (fread(&hdr, sizeof(hdr), 1, fp) != 1)
		return (feof(fp)) ? 0 : -1;

	if (strncmp(hdr.ar_fmag, ARFMAG, sizeof(hdr.ar_fmag)))
		return -1;

	siz = strtol(hdr.ar_size, NULL, 0);
	if (siz & 1)
		siz++;
	if (siz == 0)
		return -1;

	getfname(&hdr, member);

	return sizeof(hdr) + siz;
}

int
formember(FILE *fp, int (*fn)(FILE *, char *, void *), void *data)
{
	int r;
	long off;
	fpos_t pos;
	char name[SARNAM+1];

	for (;;) {
		fgetpos(fp, &pos);

		if ((off = armember(fp, name)) <= 0)
			return off;
		r = (*fn)(fp, name, data);
		if (r <= 0)
			return r;

		fsetpos(fp, &pos);
		fseek(fp, off, SEEK_CUR);
	}
}
