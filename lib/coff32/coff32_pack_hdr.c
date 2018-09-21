#include <assert.h>

#include "../../inc/coff32/filehdr.h"
#include "../../inc/coff32/coff32.h"

void
coff32_pack_hdr(packfun fun, unsigned char *buff, FILHDR *hdr)
{
	int n;

	n = (*fun)(buff,
	           "sslllss",
	           hdr->f_magic,
	           hdr->f_nscns,
	           hdr->f_timdat,
	           hdr->f_symptr,
	           hdr->f_nsyms,
	           hdr->f_opthdr,
	           hdr->f_flags);
	assert(n == FILHSZ);
}
