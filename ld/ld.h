
struct objfile {
	int (*probe)(char *fname, char *member, FILE *fp);
	int (*pass1)(char *fname, char *member, FILE *fp);
	int (*pass2)(char *fname, char *member, FILE *fp);
};
