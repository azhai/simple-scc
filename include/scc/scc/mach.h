#define NR_SYMHASH 32

typedef struct objsect Objsect;
typedef struct objsym Objsym;
typedef struct objsymdef Objsymdef;
typedef struct obj Obj;

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
	int id;
	int type;
	int align;
	unsigned flags;
	long seek;
	unsigned long long size, base;
	Objsect *next;
};

struct objsym {
	char *name;
	int type;
	int sect;
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

struct obj {
	int type;
	char *index;
	Objsym *htab[NR_SYMHASH];
	Objsym *syms;
	Objsect *secs;
	FILE *fp;
	fpos_t pos;
	int nsecs;
	int nsyms;
	void *data;
};


extern int formember(FILE *fp,
                     int (*fn)(FILE *, char *, void *),
                     void *data);

extern int objtype(FILE *fp, char **name);
extern Obj *objnew(int type);
extern void objdel(Obj *obj);
extern int objread(Obj *obj, FILE *fp);
extern Objsym *objlookup(Obj *obj, char *name, int install);
extern int objstrip(Obj *obj);
extern int objwrite(Obj *obj, FILE *fp);
extern int objsect(Obj *obj);
extern int objsyms(Obj *obj);
extern int archive(FILE *fp);
extern long armember(FILE *fp, char *member);
extern long setindex(int type, long nsyms, Objsymdef *def, FILE *fp);
extern int getindex(int type, long *nsyms, Objsymdef **def, FILE *fp);

/* TODO */
extern int objload(Obj *obj, Obj *to);
extern int objreloc(Obj *obj, char *sect, void *rel);
extern int addr2line(Obj *obj, unsigned long long addr, char *fname, int *line);
