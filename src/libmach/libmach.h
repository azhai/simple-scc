#define NBYTES 20
#define OBJ(format,arch,order) ((order) << 10 | (arch) << 5 | (format)) 
#define FORMAT(t) ((t) & 0x1f)
#define ARCH(t) (((t) >> 5) & 0x1f)
#define ORDER(t) (((t) >> 10) & 0x1f)

enum objformat {
	COFF16,
	COFF32,
	ELF32,
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

struct format {
	int (*probe)(unsigned char *buf, char **name);
	void (*strip)(Obj *obj);
	int (*new)(Obj *obj);
	int (*read)(Obj *obj, FILE *fp);
	int (*write)(Obj *obj, FILE *fp);
	void (*del)(Obj *obj);
	long (*index)(int type, long nsyms, Symdef *def, FILE *fp);
};

extern int pack(int order, unsigned char *dst, char *fmt, ...);
extern int unpack(int order, unsigned char *src, char *fmt, ...);

/* idx functions */
extern long coff32idx(int order, long nsyms, Symdef *def, FILE *fp);


/* globals */
extern struct format *objfmt[];
