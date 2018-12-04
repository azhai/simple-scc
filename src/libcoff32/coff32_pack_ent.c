#include <assert.h>

#include <scc/coff32/syms.h>
#include <scc/coff32/coff32.h>

void
coff32_pack_ent(packfun fun, unsigned char *buff, SYMENT *ent)
{
	int n;

	n = (*fun)(buff,
		   "'8lsscc",
		   &ent->n_name,
		   &ent->n_value,
		   &ent->n_scnum,
		   &ent->n_type,
		   &ent->n_sclass,
		   &ent->n_numaux);
	assert(n == SYMESZ);
}
