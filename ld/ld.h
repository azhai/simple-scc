
typedef struct obj Obj;
typedef struct symbol Symbol;

struct obj {
	char *fname;
	char *member;
	FILE *fp;
	void *filhdr;
	void *sechdr;
	char *strtbl;
	struct obj *next;
};

struct symbol {
	char *name;
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
