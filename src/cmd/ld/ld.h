/* passes */
extern void pass1(int argc, char *argv[]);
extern void pass2(int argc, char *argv[]);
extern void pass3(int argc, char *argv[]);
extern void pass4(int argc, char *argv[]);
extern void pass5(int argc, char *argv[]);

/* main.c */
extern void error(char *fmt, ...);

/* symbol.c */
extern int hasref(char *name);
extern Symbol *lookupsym(char *name);
extern int moreundef(void);
extern void listundef(void);
extern Symbol *define(Symbol *osym, Obj *obj);
extern void debugsym(void);

/* section.c */
extern Section *lookupsec(char *name);
extern void copy(Obj *obj, Section *osec, Section *sec);
extern void grow(Section *sec, int nbytes);

extern void debugsec(void);

/* globals */
extern char *filename, *membname;
extern int sflag;
extern int xflag;
extern int Xflag;
extern int rflag;
extern int dflag;
extern int gflag;
extern char *Dflag;
extern char *output, *entry;
extern Segment debug, text, rodata, data, bss;
