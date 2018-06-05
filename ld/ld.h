
#define INSTALL   1
#define NOINSTALL 0

typedef struct obj Obj;
typedef struct symbol Symbol;
typedef struct section Section;

struct obj {
	char *fname;
	char *member;
	FILE *fp;
	long offset;

	void *filhdr;
	void *scnhdr;
	void *enthdr;

	char *strtbl;
	size_t strsiz;

	int (*unpack)(unsigned char *, char *, ...);
	int align;

	struct obj *next;
};

enum symflg {
	SDEFINED = 1 << 1,
};

struct symbol {
	char *name;
	unsigned char flags;
	long size;
	TUINT base;
	TUINT value;
	Section *section;
	Obj *where;
	struct symbol *hash, *next;
};

struct section {
	char *name;
	TUINT base;
	TUINT size;
	struct section *next;
};

/* obj.c */
extern Obj *newobj(char *fname, char *member, FILE *fp);
extern Obj *add(Obj *obj);
extern void delobj(Obj *obj);
extern Section *slookup(char *name);
extern Symbol *lookup(char *name, int install);

/* main.c */
extern void outmem(void);
extern void corrupted(char *fname, char *member);
extern void redefined(Obj *obj, Symbol *sym);

/* object format */
extern Obj *probe(char *fname, char *member, FILE *fp);
extern Obj *load(Obj *obj);

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
extern Obj *objlst;
