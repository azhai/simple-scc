#include <scc/coff32/filehdr.h>
#include <scc/coff32/aouthdr.h>
#include <scc/coff32/scnhdr.h>
#include <scc/coff32/syms.h>
#include <scc/coff32/reloc.h>
#include <scc/coff32/linenum.h>

typedef struct coff32 Coff32;

struct coff32 {
	FILHDR hdr;
	AOUTHDR aout;
	SCNHDR *scns;
	SYMENT *ents;
	RELOC **rels;
	LINENO **lines;
	char *strtbl;
	unsigned long strsiz;
};

struct arch {
	char *name;
	unsigned char magic[2];
	int type;
	int flags;
};

extern int coff32new(Obj *, int);
extern int coff32read(Obj *, FILE *);
extern int coff32setidx(long, char **, long *, FILE *);
extern int coff32getidx(long *, char ***, long **, FILE *);
extern int coff32pc2line(Obj *, unsigned long long, char *, int *);
extern int coff32strip(Obj *);
extern void coff32del(Obj *);
extern int coff32write(Obj *, Map *, FILE *);
extern int coff32probe(unsigned char *, char **);
extern int coff32type(char *);

extern int coff32xsetidx(int, long , char *[], long[], FILE *);
extern int coff32xgetidx(int, long *, char ***, long **, FILE *);

extern Symbol *coff32getsym(Obj *, int *, Symbol *);
extern Section *coff32getsec(Obj *, int *, Section *);
extern Section *coff32setsec(Obj *, int *, Section *);
extern Map *coff32loadmap(Obj *, FILE *);


/* globals */
extern struct arch coff32archs[];
