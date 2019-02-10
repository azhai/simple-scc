#define NR_SYMHASH 32

typedef struct objsect Objsect;
typedef struct objsym Objsym;
typedef struct objsymdef Objsymdef;
typedef struct object Obj;

enum sectype {
	SREAD   = 1 << 0,
	SWRITE  = 1 << 1,
	SEXEC   = 1 << 2,
	SLOAD   = 1 << 3,
	SALLOC  = 1 << 4,
	SRELOC  = 1 << 5,
	SABS    = 1 << 6,
	SSHARED = 1 << 7,
};

struct objsect {
	char *name;
	int type;
	unsigned flags;
	FILE *fp;
	long offset;
	unsigned long long size;
};

struct objsym {
	char type;
	char *name;
	unsigned long long size;
	unsigned long long value;
	Objsym *next, *hash;
};

struct objsymdef {
	char *name;
	int type;
	long offset;
	Objsymdef *hash, *next;
};

struct object {
	int type;
	Objsym *htab[NR_SYMHASH];
	Objsym *head;
	fpos_t pos;
	int nsecs;
	Objsect *sections;
	void *data;
};


extern int artraverse(FILE *fp,
                      int (*fn)(FILE *, char *, void *),
                      void *data);

extern int forsym(Obj *obj,
                  int (*fn)(Objsym *sym, void *data),
                  void *data);

extern int forsect(Obj *obj,
                   int (*fn)(Objsect *sect, void *data),
                   void *data);

extern int foridx(FILE *fp,
                  int (*fn)(Objsymdef *def, void *data),
                  void *data);

extern int archive(FILE *fp);
extern long armember(FILE *fp, char *member);
extern int objtype(FILE *fp, char **name);
extern Obj *objnew(int type);
extern int objdel(Obj *obj);
extern int objreset(Obj *obj);
extern int objread(Obj *obj, FILE *fp);
extern Objsym *objlookup(Obj *obj, char *name, int install);
extern int objstrip(Obj *obj);
extern long arindex(int type, long nsyms, Objsymdef *def, FILE *fp);
extern int objwrite(Obj *obj, FILE *fp);

/* TODO */
extern int objload(Obj *obj, Obj *to);
extern int objreloc(Obj *obj, char *sect, void *rel);
extern int addr2line(Obj *obj, unsigned long long addr, char *fname, int *line);
