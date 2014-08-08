
typedef struct {
	union {
		struct {
			char type;
			char storage;
		} v;
		struct {
			short addr;
		} l;
		struct {
			char *name;
		} f;
	} u;
} Symbol;

typedef struct node {
	char op;
	char type;
	int8_t complex;
	int8_t addable;
	union {
		Symbol *sym;
		int imm;
	} u;
	struct node *left, *right;
} Node;

enum nerrors {
	EINTNUM,       /* too much internal identifiers */
	EEXTNUM,       /* too much external identifiers */
	ENODEOV,       /* node overflow */
	ESTACKO,       /* stack overflow */
	ESTACKU,       /* stack underflow */
	EEXPROV,       /* expression overflow */
	ETYPERR,       /* incorrect type in expression */
	EEXPBAL,       /* expression not balanced */
	ESYNTAX,       /* syntax error */
	ELNLINE,       /* line too long */
	EFERROR,       /* error reading from file:%s*/
	ENUMERR
};

#define FUNCTION   0
#define AUTO      'A'
#define REGISTER  'R'
#define STATIC    'T'
#define CONST     '#'
#define LABEL     'L'
#define OADD      '+'
#define OSUB      '-'

extern void error(unsigned nerror, ...);
extern void genaddable(Node *list[]);
extern void cgen(Symbol *sym, Node *list[]);
