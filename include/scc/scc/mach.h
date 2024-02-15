typedef struct segment Segment;
typedef struct section Section;
typedef struct symbol Symbol;
typedef struct objops Objops;
typedef struct obj Obj;
typedef struct map Map;
typedef struct mapsec Mapsec;

/**
 * enum sectype - Section property flags
 * @SREAD: The segment has read rights
 * @SWRITE: The segment has write rights
 * @SEXEC: The segment has execution rights
 * @SLOAD: Segment data is loaded from disk to memory
 * @SALLOC: Segment is allocated in memory
 * @SRELOC: Segment has to be relocated
 * @SABS: Segment is loaded in a specific address
 */
enum sectype {
	SREAD   = 1 << 0,
	SWRITE  = 1 << 1,
	SEXEC   = 1 << 2,
	SLOAD   = 1 << 3,
	SALLOC  = 1 << 4,
	SRELOC  = 1 << 5,
	SABS    = 1 << 6,
};

enum symtype {
	SYMNOTYPE,
	SYMOBJECT,
	SYMFUNC,
	SYMSECTION,
	SYMFILE,
	SYMCOMMON,
};

struct obj {
	char *index;
	Objops *ops;
	int type;
	long pos;
	void *data;
	Obj *next;
};

struct segment {
	char *name;
	unsigned long long base;
	unsigned long long size;
	unsigned flags;
	int index;
	int type;
	int align;
	int nsec;
	Section **sections;
};

struct section {
	char *name;
	unsigned long long base;
	unsigned long long size;

	unsigned flags;
	int index;
	int align;
	int fill;
	char type;
};

/**
 * @stype: Used internally by libmach
 * @dtype: Coff debug type
 * @flags: Used only by the assembler
 */
struct symbol {
	char *name;
	unsigned long long size;
	unsigned long long value;
	int index;
	int section;
	char type;
	int stype;
	int dtype;
	unsigned flags;
};

#ifdef stdin
extern int archive(FILE *);
extern long armember(FILE *, char *);

extern int objprobe(FILE *, char **);

extern int readobj(Obj *, FILE *);
extern int writeobj(Obj *, Map *, FILE *);

extern Map *loadmap(Obj *, FILE *);

extern int setmap(Map *,
                  char *,
                  FILE *,
                  unsigned long long,
                  unsigned long long,
                  long);

extern int setindex(int, long, char **, long *, FILE *);
extern int getindex(int, long *, char ***, long **, FILE *);

#endif

extern Map *newmap(Map *, int);
extern Map *remap(Map *, int);

extern Obj *newobj(int);
extern void delobj(Obj *);

extern int strip(Obj *);
extern int pc2line(Obj *, unsigned long long, char *, int *);
extern int rebase(Obj *, int, unsigned long long);

extern int findsec(Map *, char *);

extern Symbol *getsym(Obj *, int *, Symbol *);
extern Section *getsec(Obj *, int *, Section *);
