/*
 * First 3 bits of flags in segments and symbols are for the
 * type of segment
 */
enum symflags {
	FREG    = 1 << 0,
	FSECT   = 1 << 1,
	FSYM    = 1 << 2,
	FCOMMON = 1 << 3,
	FEXTERN = 1 << 4,
	FDEF    = 1 << 5,
	FGLOBAL = 1 << 6,
	FABS    = 1 << 7,
};

/*
 * some systems polute the namespace defining BIG_ENDIAN and
 * LITTLE_ENDIAN
 */
#undef BIG_ENDIAN
#undef LITTLE_ENDIAN

enum endianess {
	BIG_ENDIAN    = -1,
	LITTLE_ENDIAN = 1
};

enum common_args {
	AIMM = 1,
	ASTR,
	AREG,
	ANUMBER,
	AIMM2,
	AIMM3,
	AIMM5,
	AIMM8,
	AIMM16,
	AIMM32,
	AIMM64,
	AINDIR,
	AINDEX,
	ADIRECT,
	AREG_OFF,
	ASYM,
	AOPT,
	AREP,
	AMAX,
};

enum tokens {
	EOS = -1,
	IDEN = 1,
	NUMBER,
	REG,
	STRING,
	MINUS,
	SHL,
	SHR,
	GE,
	LE,
};

#define MAXSYM 63

typedef struct reloc Reloc;
typedef struct ins Ins;
typedef struct op Op;
typedef struct node Node;
typedef void Format(Op *, Node **);

struct line {
	char *label;
	char *op;
	char *args;
};

struct ins {
	int begin, end;
	char *str;
};

struct reloc {
	size_t offset;
	Symbol *sym;
	unsigned char flags;
	unsigned char size;
	unsigned char nbits;
	unsigned char shift;
};

struct op {
	unsigned char flags;
	unsigned char size;
	void (*format)(Op *, Node **);
	unsigned char *bytes;
	unsigned char *args;
};

struct node {
	unsigned char op;
	unsigned char addr;
	struct symbol *sym;
	struct node *left;
	struct node *right;
};

union yylval {
	TUINT val;
	Symbol *sym;
};

/* symbol.c */
extern void cleansecs(void);
extern void isecs(void);
extern void emit(char *, int);
extern Section *defsec(char *, char *);
extern Symbol *tmpsym(TUINT);
extern void killtmp(void);
extern int toobig(Node *, int);
extern void dumpstab(char *);
extern Symbol *lookup(char *);
extern Symbol *deflabel(char *);
extern unsigned long long getpc(void);

/* parser.c */
extern Node **getargs(char *);
extern void error(char *, ...);
/* Avoid errors in files where stdio is not included */
#ifdef stdin
extern int nextline(struct line *);
#endif
extern void unexpected(void);
extern void expect(int);
int next(void);
#define accept(t) (yytoken == (t) ? next() : 0)
extern void regctx(int mode);
extern Node *getreg(void);
extern Node *operand(char **);
extern void addinput(char *);
extern int delinput(void);
extern int ahead(void);

/* expr.c */
extern Node *expr(void);
extern void deltree(Node *);
extern Node *node(int, Node *, Node *);

/* proc.c */
extern void iarch(void);
extern int match(Op *, Node **);
extern Node *moperand(void);

/* ins.c */
extern char *tobytes(TUINT, int, int);

/* binfmt.c */
extern void writeout(char *);

/*
 * Definition of global variables
 */
extern unsigned long M, S, K;
extern short hashmap[];
extern Section *cursec;
extern Ins instab[];
extern Op optab[];
extern int pass;
extern TUINT maxaddr;
extern int endian;
extern Symbol *linesym;
extern char *infile, *outfile;
extern int endpass;
extern int yytoken;
extern size_t yylen;
extern union yylval yylval;
extern char yytext[];
