#define NR_SYMHASH 32

typedef struct objsect Objsect;
typedef struct objsym Objsym;
typedef struct objsymdef Objsymdef;
typedef struct objops Objops;
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

struct objops {
	int (*probe)(unsigned char *buf, char **name);
	int (*new)(Obj *obj);
	int (*read)(Obj *obj, FILE *fp);
	int (*addr2line)(Obj *, unsigned long long , char *, int *);
	int (*strip)(Obj *obj);
	void (*del)(Obj *obj);
	int (*write)(Obj *obj, FILE *fp);
	int (*setidx)(long nsyms, Objsymdef *def, FILE *fp);
	int (*getidx)(long *nsyms, Objsymdef **def, FILE *fp);
};

struct obj {
	int type;
	Objops *ops;
	char *index;
	Objsym *htab[NR_SYMHASH];
	Objsym *syms;
	Objsect *secs;
	long pos;
	int nsecs;
	int nsyms;
	void *data;
};

extern int archive(FILE *fp);
extern int formember(FILE *fp,
                     int (*fn)(FILE *, char *, void *),
                     void *data);

extern int objtype(FILE *fp, char **name);
extern Obj *objnew(int type);
extern Objsym *objlookup(Obj *obj, char *name, int install);
extern int objpos(Obj *obj, FILE *fp, long pos);


/* TODO */
extern int objaddseg(Obj *obj, void *seg);
extern int objreloc(Obj *obj, char *sect, void *rel);
