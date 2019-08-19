struct obj;
struct objsym;

typedef struct objlst Objlst;
typedef struct symbol Symbol;
typedef struct section Section;
typedef struct segment Segment;

struct section {
	char *name;
	unsigned long long base;
	unsigned long size;
	unsigned flags;
	int type;
	FILE *fp;
	Section *hash;
	Section *next;
};

struct segment {
	char *name;
	int type;
	unsigned nsec;
	unsigned long long base;
	unsigned long size;
	Section **sections;
};

struct objlst {
	struct obj *obj;
	struct objlst *next;
};

struct symbol {
	char *name;
	struct obj *obj;
	struct objsym *def;
	unsigned long long size, value;
	struct symbol *next, *prev;
	struct symbol *hash;
};

/* passes */
extern void pass1(int argc, char *argv[]);
extern void pass2(int argc, char *argv[]);
extern void pass3(int argc, char *argv[]);
extern void pass4(int argc, char *argv[]);

/* main.c */
extern char *errstr(void);
extern void error(char *fmt, ...);

/* symbol.c */
extern Symbol *lookup(char *name);
extern Symbol *install(char *name);
extern Section *section(char *name);
extern void debugsym(void);
extern void debugsec(void);

/* globals */
extern char *filename, *membname;
extern int sflag;
extern int xflag;
extern int Xflag;
extern int rflag;
extern int dflag;
extern int gflag;
extern char *Dflag;
extern Objlst *objhead;
extern Section *sechead;
extern Segment text, rodata, data, bss;
