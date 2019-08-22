#include <scc/coff32/filehdr.h>
#include <scc/coff32/aouthdr.h>
#include <scc/coff32/scnhdr.h>
#include <scc/coff32/syms.h>
#include <scc/coff32/reloc.h>
#include <scc/coff32/linenum.h>

typedef struct coff32 Coff32;

struct arch {
	char *name;
	unsigned char magic[2];
	int type;
};

struct coff32 {
	FILHDR hdr;
	AOUTHDR *aout;
	SCNHDR *scns;
	SYMENT *ents;
	RELOC **rels;
	LINENO **lines;
	char *strtbl;
	unsigned long strsiz;
};

extern int coff32new(Obj *obj);
extern int coff32read(Obj *obj, FILE *fp);
