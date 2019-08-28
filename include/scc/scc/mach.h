typedef struct symbol Symbol;
typedef struct section Section;
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

struct obj {
	char *index;
	Objops *ops;
	int type;
	long pos;
	void *data;
};

struct section {
	char *name;
	unsigned long long base;
	unsigned long long size;
	unsigned flags;
	int type;
	int index;
	int align;
};

struct symbol {
	char *name;
	unsigned long long size;
	unsigned long long value;
	int index;
	char class;
	char type;
};

extern int archive(FILE *fp);
extern long armember(FILE *fp, char *member);

extern int objtype(FILE *fp, char **name);
extern Obj *newobj(int type);
extern void delobj(Obj *obj);

extern int readobj(Obj *obj, FILE *fp);
extern int writeobj(Obj *obj, FILE *fp);

extern int strip(Obj *obj);

extern Symbol *getsym(Obj *obj, long *index, Symbol *sym);
extern Section *getsec(Obj *obj, long *index, Section *sec);

extern int setindex(int, long, char **, long *, FILE *);
extern int getindex(int, long *, char ***, long **, FILE *);
