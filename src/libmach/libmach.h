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

enum deltype {
	GENERICDEL = 1 << 0,
	TARGETDEL  = 1 << 1,
};

typedef int (*newfun_t)(Obj *obj);
typedef int (*readfun_t)(Obj *obj, FILE *fp);
typedef void (*delfun_t)(Obj *new);
typedef void (*stripfun_t)(Obj *obj);
typedef int (*probefun_t)(unsigned char *buf, char **name);
typedef int (*writefun_t)(Obj *obj, FILE *fp);
typedef long (*setidxfun_t)(int, long, Objsymdef *, FILE *);
typedef int (*getidxfun_t)(int t, long *n, Objsymdef **def, FILE *fp);
typedef int (*getsectfun_t)(Obj *obj);
typedef char *(*namidxfun_t)(void);
typedef int (*getsymsfun_t)(Obj *obj);

/* common functions */
extern int pack(int order, unsigned char *dst, char *fmt, ...);
extern int unpack(int order, unsigned char *src, char *fmt, ...);
extern int objpos(Obj *obj, FILE *fp, long pos);
extern int objfree(Obj *obj, int what);

/* coff32 functions */
extern long coff32index(int type, long nsyms, Objsymdef *head, FILE *fp);
extern int coff32new(Obj *obj);
extern void coff32del(Obj *obj);
extern int coff32read(Obj *obj, FILE *fp);
extern int coff32write(Obj *obj, FILE *fp);
extern void coff32strip(Obj *obj);
extern int coff32probe(unsigned char *buf, char **name);

extern long coff32setindex(int type, long nsymbols, Objsymdef *head, FILE *fp);
extern long coff32setidx(int order, long nsyms, Objsymdef *head, FILE *fp);

extern int coff32getindex(int type, long *nsyms, Objsymdef **def, FILE *fp);
extern int coff32getidx(int order, long *nsyms, Objsymdef **def, FILE *fp);

extern int coff32getsect(Obj *obj);

extern char *coff32namidx(void);
extern int coff32getsyms(Obj *obj);
