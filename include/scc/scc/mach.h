#define NR_SYMHASH 32

typedef struct objsec Objsec;
typedef struct symbol Symbol;
typedef struct objseg Objseg;
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

struct objsec {
	char *name;
	int id;
	int type;
	int align;
	unsigned flags;
	long seek;
	unsigned long long size, base;
	Objsec *next;
};

struct symbol {
	char *name;
	unsigned long long size;
	unsigned long long value;
	void *aux;
	int index;
	char class;
	char type;
};

struct objseg {
	unsigned long long size;
	unsigned long long value;
};

struct objops {
	int (*probe)(unsigned char *buf, char **name);
	int (*new)(Obj *obj);
	void (*del)(Obj *obj);
	int (*read)(Obj *obj, FILE *fp);
	int (*write)(Obj *obj, FILE *fp);
	int (*strip)(Obj *obj);
	int (*addr2line)(Obj *, unsigned long long , char *, int *);
	int (*getsym)(Obj *obj, long *index, Symbol *sym);
	int (*setidx)(long nsyms, char *names[], long offset[], FILE *fp);
	int (*getidx)(long *nsyms, char ***names, long **offset, FILE *fp);
};

struct obj {
	char *index;
	Objops *ops;
	int type;
	long pos;
	void *data;
};

extern int archive(FILE *fp);
extern long armember(FILE *fp, char *member);

extern int objtype(FILE *fp, char **name);
extern Obj *objnew(int type);
extern int readobj(Obj *obj, FILE *fp);
extern int getsym(Obj *obj, long *index, Symbol *sym);
