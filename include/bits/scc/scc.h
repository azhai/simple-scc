#include <stddef.h>

extern int enadebug;

#ifndef NDEBUG
#define DBG(...) dbg(__VA_ARGS__)
#define DBGON() (enadebug = 1)
#else
#define DBG(...)
#define DBGON()
#endif

#define TINT        long long
#define TUINT       unsigned long long
#define TUINT_MAX   ULLONG_MAX
#define TINT_MAX    LLONG_MAX
#define TFLOAT      double
#define SIZET       size_t

struct items {
	char **s;
	unsigned n;
};

#ifdef CLOCKS_PER_SEC
struct fprop {
	unsigned uid;
	unsigned gid;
	unsigned long mode;
	long size;
	time_t time;
};
#endif

typedef struct alloc Alloc;

extern void die(const char *fmt, ...);
extern void dbg(const char *fmt, ...);
extern void newitem(struct items *items, char *item);
extern void *xmalloc(size_t size);
extern void *xcalloc(size_t nmemb, size_t size);
extern char *xstrdup(const char *s);
extern void *xrealloc(void *buff, size_t size);
extern Alloc *alloc(size_t size, size_t nmemb);
extern void dealloc(Alloc *allocp);
extern void *new(Alloc *allocp);
extern void delete(Alloc *allocp, void *p);
extern int casecmp(const char *s1, const char *s2);
extern unsigned genhash(char *name);
extern char *canonical(char *);

#ifdef CLOCKS_PER_SEC
extern long long fromepoch(time_t);
extern time_t totime(long long);
extern int getstat(char *, struct fprop *);
extern int setstat(char *, struct fprop *);
#endif
