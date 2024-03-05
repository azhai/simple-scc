/*
 * some systems define macros with this name even when
 * they should be c99 compliant.
 */
#undef LITTLE_ENDIAN
#undef BIG_ENDIAN

#define NBYTES 32
#define OBJ(format,arch,order) ((order) << 10 | (arch) << 5 | (format)) 
#define FORMAT(t) ((t) & 0x1f)
#define ARCH(t) (((t) >> 5) & 0x1f)
#define ORDER(t) (((t) >> 10) & 0x1f)


enum objformat {
	COFF32,
	ELF64,
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
	int (*type)(char *);
	int (*probe)(unsigned char *, char **);

	int (*new)(Obj *);
	void (*del)(Obj *);

	int (*read)(Obj *, FILE *);
	int (*write)(Obj *, Map *, FILE *);

	int (*strip)(Obj *);
	int (*pc2line)(Obj *, unsigned long long , char *, int *);

	Map *(*loadmap)(Obj *, FILE *);

	Symbol *(*getsym)(Obj *, int *, Symbol *);
	Section *(*getsec)(Obj *, int *, Section *);
	Section *(*setsec)(Obj *, int *, Section *);

	int (*setidx)(long, char *[], long[], FILE *);
	int (*getidx)(long *, char ***, long **, FILE *);
};

struct map {
	int n;
	struct mapsec {
		char *name;
		FILE *fp;
		unsigned long long begin;
		unsigned long long end;
		long offset;
	} sec[];
};

/* common functions */
extern int pack(int order, unsigned char *dst, char *fmt, ...);
extern int unpack(int order, unsigned char *src, char *fmt, ...);
extern int objpos(Obj *obj, FILE *fp, long pos);

/* globals */
extern Objops *objops[];
extern Objops coff32;
extern Objops elf64;
