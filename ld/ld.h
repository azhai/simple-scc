
typedef struct obj Obj;

struct obj {
	char *fname;
	void *hdr;
	char *strtbl;
	struct obj *next;
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
