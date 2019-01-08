struct fprop {
	unsigned uid;
	unsigned gid;
	unsigned mode;
	long size;
	time_t time;
};

extern time_t totime(long long t);
extern int getstat(char *fname, struct fprop *prop);
extern int setstat(char *fname, struct fprop *prop);
