
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

	Symbol **symbols;
	Symbol **sections;

	char *strtbl;
	size_t strsiz;

	int (*unpack)(unsigned char *, char *, ...);
	int align;
	int define;

	struct obj *next, *prev;
};

struct symbol {
	char *name;
	char type;
	short flags;
	long size;
	TUINT base;
	TUINT value;
	Obj *where;
	struct symbol *hash;
};

struct objfmt {
	Obj  *(*probe)(char *fname, char *member, FILE *fp);
	void (*pass1)(Obj *obj);
	void (*pass2)(Obj *obj);
};

/* obj.c */
extern Obj *newobj(char *fname, char *member, FILE *fp);
extern void delobj(Obj *obj);
extern void pop(Obj *obj);
extern void push(Obj *obj);
extern Symbol *lookup(char *name);

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
