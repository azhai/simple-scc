
struct symbol {
	char *name;
	int type;
	unsigned long long value;
	unsigned long size;
};

struct objfile {
	int (*probe)(FILE *fp);
	void (*nm)(char *fname, char *member, FILE *fp);
};

/* main.c */
extern void printsyms(char *, char *, struct symbol *, size_t );
