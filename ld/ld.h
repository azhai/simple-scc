
#define INSTALL   1
#define NOINSTALL 0

typedef struct obj Obj;
typedef struct symbol Symbol;
typedef struct objfmt Fmt;

struct obj {
	char *fname;
	char *member;
	FILE *fp;
	Fmt *fmt;
	long offset;

	void *filhdr;
	void *scnhdr;
	void *enthdr;

	Symbol **symbols;
	Symbol **sections;

	char *strtbl;
	size_t strsiz;

	int (*unpack)(unsigned char *, char *, ...);
	int align;

	struct obj *next, *prev;
};

enum symflgs {
	SSECT = 1 << 0,
};

struct symbol {
	char *name;
	char type;
	short flags;
	long size;
	TUINT base;
	TUINT value;
	Obj *where;
	struct symbol *hash, *next;
};

struct objfmt {
	Obj  *(*probe)(char *fname, char *member, FILE *fp);
	void (*pass1)(Obj *obj);
	void (*pass2)(Obj *obj);
};

/* obj.c */
extern Obj *newobj(char *fname, char *member, FILE *fp);
extern void add(Obj *obj);
extern void delobj(Obj *obj);
extern void newsect(Symbol *sym);
extern Symbol *lookup(char *name, int install);

/* main.c */
extern void outmem(void);

/*
 * Definition of globals variables
 */
extern int pass;
extern int sflag;
extern int xflag;
extern int Xflag;
extern int rflag;
extern int dflag;
extern int gflag;
