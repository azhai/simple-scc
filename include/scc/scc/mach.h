#define NR_SYMHASH 32

typedef struct section Section;
typedef struct symbol Symbol;
typedef struct symdef Symdef;
typedef struct object Obj;

enum sectype {
	SREAD   = 1 << 0,
	SWRITE  = 1 << 1,
	SEXEC   = 1 << 2,
	SNOLOAD = 1 << 3,
	SFILE   = 1 << 4,
	SABS    = 1 << 5,
	SBLOB   = 1 << 6,
};

struct section {
	char *name;
	unsigned flags;
	FILE *fp;
	long offset;
	unsigned long long size;
	Section *next;
};

struct symbol {
	char type;
	char *name;
	unsigned long long size;
	unsigned long long value;
	Symbol *next;
	Symbol *hash;
};

struct symdef {
	char *name;
	int type;
	long offset;
	Symdef *hash, *next;
};

struct object {
	int type;
	Symbol *htab[NR_SYMHASH];
	Symbol *head;
	fpos_t pos;
	Section *sections;
	void *data;
};

extern int archive(FILE *fp);
extern long armember(FILE *fp, char *member);

extern int artraverse(FILE *fp,
                      int (*fn)(FILE *, char *, void *),
                      void *data);

extern int objtype(FILE *fp, char **name);
extern Obj *objnew(int type);
extern int objdel(Obj *obj);
extern int objreset(Obj *obj);
extern int objread(Obj *obj, FILE *fp);
extern Symbol *objlookup(Obj *obj, char *name);
extern int objtraverse(Obj *obj, int (*fn)(Symbol *sym, void *data), void *data);
extern int objstrip(Obj *obj);
extern int objsize(Obj *obj,
                   unsigned long long *text,
                   unsigned long long *data,
                   unsigned long long *bss);
extern long arindex(int type, long nsyms, Symdef *def, FILE *fp);

/* TODO */
extern int objload(Obj *obj, Obj *to);
extern int objreloc(Obj *obj, char *sect, void *rel);
extern int objwrite(Obj *obj, FILE *fp);
extern int addr2line(Obj *obj, unsigned long long addr, char *fname, int *line);
