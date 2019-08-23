#define NR_SYMHASH 32

typedef struct objsymdef Objsymdef;
typedef struct objsec Objsec;
typedef struct objsym Objsym;
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

struct objsym {
	char *name;
	int type;
	int sec;
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
	int (*addseg)(Obj *obj, void *seg);
	int (*strip)(Obj *obj);
	int (*addr2line)(Obj *, unsigned long long , char *, int *);
	int (*setidx)(long nsyms, char *names[], long offset[], FILE *fp);
	int (*getidx)(long *nsyms, char ***names, long **offset, FILE *fp);
};

struct obj {
	int type;
	Objops *ops;
	char *index;
	Objsym *htab[NR_SYMHASH];
	Objsym *syms;
	Objsec *secs;
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
