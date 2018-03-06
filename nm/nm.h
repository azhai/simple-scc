
struct symbol {
	char *name;
	int type;
	unsigned long long off;
	unsigned long size;
};

struct objfile {
	int (*probe)(FILE *fp);
	void (*nm)(char *fname, FILE *fp);
};

/* main.c */
extern void print(char *file, char *member, struct symbol *sym);
