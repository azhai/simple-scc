#undef assert

extern void __assert(char *, char *, long);
extern void assert(int);

#ifndef NDEBUG
# define assert(exp) ((exp) ? (void) 0 : __assert(#exp, __FILE__, __LINE__))
#else
# define assert(exp) ((void) 0)
#endif
