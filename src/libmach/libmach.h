#define NBYTES 20
#define OBJ(format,arch,order) ((order) << 10 | (arch) << 5 | (format)) 
#define FORMAT(t) ((t) & 0x1f)
#define ARCH(t) (((t) >> 5) & 0x1f)
#define ORDER(t) (((t) >> 10) & 0x1f)

typedef struct objops Objops;

enum objformat {
	COFF32,
	NFORMATS,
};

enum objarch {
	ARCH286,
	ARCH386,
	ARCHAMD64,
	ARCHZ80,
	ARCHARM32,
	ARCHARM64,
};

enum order {
	LITTLE_ENDIAN,
	BIG_ENDIAN,
};

struct objops {
	int (*probe)(unsigned char *buf, char **name);

	int (*new)(Obj *obj);
	void (*del)(Obj *obj);

	int (*read)(Obj *obj, FILE *fp);
	int (*write)(Obj *obj, FILE *fp);

	int (*strip)(Obj *obj);
	int (*addr2line)(Obj *, unsigned long long , char *, int *);

	Symbol *(*getsym)(Obj *obj, long *index, Symbol *sym);
	Section *(*getsec)(Obj *obj, long *index, Section *sec);

	int (*setidx)(long nsyms, char *names[], long offset[], FILE *fp);
	int (*getidx)(long *nsyms, char ***names, long **offset, FILE *fp);
};

struct obj {
	char *index;
	struct objops *ops;
	int type;
	long pos;
	void *data;
};

/* common functions */
extern int pack(int order, unsigned char *dst, char *fmt, ...);
extern int unpack(int order, unsigned char *src, char *fmt, ...);
extern int objpos(Obj *obj, FILE *fp, long pos);

/* globals */
extern Objops *objops[];
extern Objops coff32;
