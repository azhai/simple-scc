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
	int (*strip)(Obj *obj);
	int (*new)(Obj *obj);
	int (*read)(Obj *obj, FILE *fp);
	int (*write)(Obj *obj, FILE *fp);
	void (*del)(Obj *obj);
	int (*size)(Obj *obj,
                unsigned long long *,
                unsigned long long *,
                unsigned long long *)
};

extern int pack(int order, unsigned char *dst, char *fmt, ...);
extern int unpack(int order, unsigned char *src, char *fmt, ...);


/* globals */
extern struct format *objfmt[];
