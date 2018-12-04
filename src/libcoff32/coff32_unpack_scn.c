#include <assert.h>

#include <scc/coff32/scnhdr.h>
#include <scc/coff32/coff32.h>

void
coff32_unpack_scn(packfun fun, unsigned char *buff, SCNHDR *scn)
{
	int n;

	n = (*fun)(buff,
                  "'8llllllssl",
	          scn->s_name,
	          &scn->s_paddr,
	          &scn->s_vaddr,
	          &scn->s_size,
	          &scn->s_scnptr,
	          &scn->s_relptr,
	          &scn->s_lnnoptr,
	          &scn->s_nrelloc,
	          &scn->s_nlnno,
	          &scn->s_flags);
	assert(n == SCNHSZ);
}
