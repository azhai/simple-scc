
#define INPUTSIZ LINESIZ
#ifndef PREFIX
#define PREFIX "/usr/"
#endif

#define GLOBALCTX 0

/*
 * Definition of structures
 */
typedef struct type Type;
typedef struct symbol Symbol;
typedef struct caselist Caselist;
typedef struct node Node;
typedef struct input Input;

/*
 * TODO: Some of the data stored in type is shared with
 *       cc2, so it should be stored in a table shared
 *       between both programs, and dependant of the target.
 */
struct type {
	unsigned char op;           /* type builder operator */
	unsigned char ns;           /* namespace for struct members */
	short id;                   /* type id, used in dcls */
	char letter;                /* letter of the type */
	bool defined : 1;           /* type defined */
	bool sign : 1;              /* signess of the type */
	bool printed : 1;           /* the type already was printed */
	size_t size;                /* sizeof the type */
	size_t align;               /* align of the type */
	Type *type;                 /* base type */
	Symbol *tag;                /* symbol of the strug tag */
	Type *next;                 /* next element in the hash */
	union {
		Type **pars;            /* Function type parameters */
		Symbol **fields;        /* fields of aggregate type */
	} p;
	union {
		unsigned char rank;     /* convertion rank */
		short elem;             /* number of type parameters */
	} n;
};

struct symbol {
	char *name;
	Type *type;
	unsigned short id;
	unsigned char ctx;
	unsigned char ns;
	unsigned char token;
	short flags;
	union {
		TINT i;
		TUINT u;
		TFLOAT f;
		char *s;
		unsigned char token;
		Symbol **pars;
	} u;
	struct symbol *next;
	struct symbol *hash;
};

struct node {
	unsigned char op;
	Type *type;
	Symbol *sym;
	bool lvalue : 1;
	bool symbol: 1;
	bool constant : 1;
	struct node *left, *right;
};

struct scase {
	Symbol *label;
	Node *expr;
	struct scase *next;
};

struct caselist {
	short nr;
	Symbol *deflabel;
	struct scase *head;
};

struct yystype {
	Symbol *sym;
	unsigned char token;
};

struct input {
	char *fname;
	FILE *fp;
	char *line, *begin, *p;
	struct input *next;
	unsigned short nline;
};

/*
 * Definition of enumerations
 */

/* recovery points */
enum {
	END_DECL,
	END_LDECL,
	END_COMP,
	END_COND
};

/* type constructors */
enum {
	FTN = 1,
	PTR,
	ARY
};

/* namespaces */
enum {
	NS_IDEN = 1,
	NS_TAG,
	NS_LABEL,
	NS_CPP,
	NS_KEYWORD,
	NS_CPPCLAUSES,
	NS_STRUCTS
};

/* symbol flags */
enum {
	ISAUTO     =       1,
	ISREGISTER =       2,
	ISDECLARED =       4,
	ISFIELD    =       8,
	ISEXTERN   =      16,
	ISUSED     =      32,
	ISCONSTANT =      64,
	ISGLOBAL   =     128,
	ISPRIVATE  =     256,
	ISLOCAL    =     512,
	ISEMITTED  =    1024,
	ISDEFINED  =    2048,
	ISSTRING   =    4096
};


/* lexer mode, compiler or preprocessor directive */
enum {
	CCMODE,
	CPPMODE
};

/* input tokens */
enum tokens {
	TQUALIFIER = 128,
	TYPE,
	IDEN,
	SCLASS,
	CONSTANT,
	SIZEOF,
	INDIR,
	INC,
	DEC,
	SHL,
	SHR,
	LE,
	GE,
	EQ,
	NE,
	AND,
	OR,
	MUL_EQ,
	DIV_EQ,
	MOD_EQ,
	ADD_EQ,
	SUB_EQ,
	AND_EQ,
	XOR_EQ,
	OR_EQ,
	SHL_EQ,
	SHR_EQ,
	ELLIPSIS,
	CASE,
	DEFAULT,
	IF,
	ELSE,
	SWITCH,
	WHILE,
	DO,
	FOR,
	GOTO,
	VOID,
	FLOAT,
	INT,
	BOOL,
	STRUCT,
	UNION,
	CHAR,
	DOUBLE,
	SHORT,
	LONG,
	LLONG,
	COMPLEX,
	CONST,
	VOLATILE,
	RESTRICT,
	TYPEDEF,
	EXTERN,
	STATIC,
	AUTO,
	REGISTER,
	ENUM,
	TYPEIDEN,
	UNSIGNED,
	SIGNED,
	CONTINUE,
	BREAK,
	RETURN,
	DEFINE,
	INCLUDE,
	LINE,
	PRAGMA,
	ERROR,
	IFDEF,
	ELIF,
	IFNDEF,
	UNDEF,
	ENDIF,
	EOFTOK
};

/* operations */
enum op {
	OPTR,
	OADD,
	OMUL,
	OSUB,
	OINC,
	ODEC,
	ODIV,
	OMOD,
	OSHL,
	OSHR,
	OBAND,
	OBXOR,
	OBOR,
	OASSIGN,
	OA_MUL,
	OA_DIV,
	OA_MOD,
	OA_ADD,
	OA_SUB,
	OA_SHL,
	OA_SHR,
	OA_AND,
	OA_XOR,
	OA_OR,
	OADDR,
	ONEG,
	OCPL,
	OEXC,
	OCOMMA,
	OCAST,
	OSYM,
	OASK,
	OCOLON,
	OFIELD,
	OLABEL,
	ODEFAULT,
	OCASE,
	OJUMP,
	OBRANCH,
	OEXPR,
	OEFUN,
	OELOOP,
	OBLOOP,
	OFUN,
	OPAR,
	OCALL,
	ORET,
	ODECL,
	OSWITCH,
	OAND,
	OOR,
	OEQ,
	ONE,
	OLT,
	OGE,
	OLE,
	OGT
};

/* error.c */
extern void error(char *fmt, ...);
extern void warn(char *fmt, ...);
extern void unexpected(void);
extern void printerr(char *fmt, ...);
extern void cpperror(char *fmt, ...);

/* types.c */
extern bool eqtype(Type *tp1, Type *tp2);
extern Type *ctype(unsigned type, unsigned sign, unsigned size);
extern Type *mktype(Type *tp, unsigned op, short nelem, Type *data[]);
extern Type *duptype(Type *base);

/* symbol.c */
extern void dumpstab(char *msg);
extern Symbol *lookup(unsigned ns);
extern Symbol *nextsym(Symbol *sym, unsigned ns);
extern Symbol *install(unsigned ns, Symbol *sym);
extern Symbol *newsym(unsigned ns);
extern void pushctx(void), popctx(void);
extern void ikeywords(void);
extern void delmacro(Symbol *sym);

/* stmt.c */
extern void compound(Symbol *lbreak, Symbol *lcont, Caselist *lswitch);

/* decl.c */
extern Type *typename(void);
extern void decl(void);

/* lex.c */
extern char ahead(void);
extern unsigned next(void);
extern bool moreinput(void);
extern void expect(unsigned tok);
extern void discard(void);
extern bool addinput(char *fname);
extern void setnamespace(int ns);
extern void setsafe(int type);
extern void ilex(char *fname);
#define accept(t) ((yytoken == (t)) ? next() : 0)

/* code.c */
extern void emit(unsigned, void *);
extern Node *node(unsigned op, Type *tp, Node *left, Node *rigth);
extern Node *varnode(Symbol *sym);
extern Node *constnode(Symbol *sym);
extern Node *sizeofnode(Type *tp);
extern void freetree(Node *np);
#define BTYPE(np) ((np)->type->op)

/* expr.c */
extern Node *expr(void), *negate(Node *np), *constexpr(void);
extern Node *convert(Node *np, Type *tp1, char iscast);
extern Node *eval(Node *np), *iconstexpr(void), *condition(void);
extern Node *exp2cond(Node *np, char neg);

/* cpp.c */
extern void icpp(void);
extern bool cpp(void);
extern bool expand(char *begin, Symbol *sym);

/*
 * Definition of global variables
 */
extern struct yystype yylval;
extern char yytext[];
extern unsigned yytoken;
extern unsigned short yylen;
extern int cppoff, disexpand;
extern unsigned cppctx;
extern Input *input;
extern int lexmode;
extern unsigned curctx;
extern Symbol *curfun, *zero, *one;

extern Type *voidtype, *pvoidtype, *booltype,
            *uchartype,   *chartype,
            *uinttype,    *inttype,     *sizettype,
            *ushortype,   *shortype,
            *longtype,    *ulongtype,
            *ullongtype,  *llongtype,
            *floattype,   *doubletype,  *ldoubletype;
