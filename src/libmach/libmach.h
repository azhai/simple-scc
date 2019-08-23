#define NBYTES 20
#define OBJ(format,arch,order) ((order) << 10 | (arch) << 5 | (format)) 
#define FORMAT(t) ((t) & 0x1f)
#define ARCH(t) (((t) >> 5) & 0x1f)
#define ORDER(t) (((t) >> 10) & 0x1f)

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

/* common functions */
extern int pack(int order, unsigned char *dst, char *fmt, ...);
extern int unpack(int order, unsigned char *src, char *fmt, ...);
extern void objfree(Obj *obj);

/* coff32 functions */
/* TODO: Move this functions to a coff32 files */
extern int coff32write(Obj *obj, FILE *fp);
extern int coff32probe(unsigned char *buf, char **name);

extern char *coff32namidx(void);
extern int coff32sync(Obj *obj);

/* globals */
extern Objops *objops[];
extern Objops coff32;
