
typedef struct obj Obj;
typedef struct symbol Symbol;

struct obj {
	char *fname;
	void *hdr;
	char *strtbl;
	struct obj *next;
};

struct symbol {
	char *name;
};

#ifdef stdin
struct objfile {
	int (*probe)(char *fname, char *member, FILE *fp);
	void (*pass1)(char *fname, char *member, FILE *fp);
	void (*pass2)(char *fname, char *member, FILE *fp);
};
#endif

/* obj.c */
extern Obj *newobj(char *fname);
extern Symbol *lookup(char *name);
