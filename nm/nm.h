
struct symbol {
	char *name;
	int type;
	unsigned long long off;
	unsigned long size;
};

/* main.c */
extern void print(char *file, char *member, struct symbol *sym);

/* object format file */
extern void nm(char *fname, char *member, FILE *fp);
extern int object(FILE *fp);

extern int radix;
extern int Pflag;
extern int Aflag;
extern int vflag;
extern int gflag;
extern int uflag;
extern int arflag;
