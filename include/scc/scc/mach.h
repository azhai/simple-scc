typedef struct symbol Symbol;
typedef struct object Obj;

struct symbol {
	char type;
	char *name;
	unsigned long long size;
	unsigned long long value;
};

struct object {
	int type;
	Symbol *symtbl;
	unsigned long nsym;
	unsigned long cursym;
	fpos_t pos;
	void *data;
};

extern int archive(FILE *fp);
extern int armember(FILE *fp, char *member);

extern int artraverse(FILE *fp,
                      int (*fn)(FILE *, char *, void *),
                      void *data);

extern int objtest(FILE *fp, char **name);
extern int objopen(FILE *fp, int type, Obj *obj);
extern int objread(FILE *fp, Obj *obj, int (*filter)(Symbol *));
extern void objclose(Obj *obj);
