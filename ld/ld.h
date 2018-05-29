
typedef struct obj Obj;
typedef struct symbol Symbol;

struct obj {
	char *fname;
	char *member;
	FILE *fp;
	void *filhdr;
	void *scnhdr;
	Symbol **symbols;
	char *strtbl;
	size_t strsiz;
	struct obj *next;
};

struct symbol {
	char *name;
	char type;
	short flags;
	long size;
	TUINT base;
	TUINT value;
	struct symbol *hash;
};

struct objfile {
	int (*probe)(char *fname, char *member, FILE *fp);
	void (*pass1)(char *fname, char *member, FILE *fp);
	void (*pass2)(char *fname, char *member, FILE *fp);
};

/* obj.c */
extern Obj *newobj(char *fname, char *member);
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
