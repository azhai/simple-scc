
struct symbol {
	char *name;
	int type;
	unsigned long long value;
	unsigned long size;
};

struct objfile {
	int (*probe)(char *fname, char *member, FILE *fp);
	int (*nm)(char *fname, char *member, FILE *fp);
};

/* main.c */
extern void printsyms(char *, char *, struct symbol *, size_t );
