#ifndef _STDLIB_H
#define _STDLIB_H

#define _NEED_NULL
#define _NEED_SIZET
#define _NEED_WCHART
#include <sys/cdefs.h>
#include <sys/stdlib.h>
#include <arch/cdefs.h>
#include <arch/stdlib.h>

#define _ATEXIT_MAX 32

#define MB_CUR_MAX 1
#define RAND_MAX 32767

typedef struct {
	int quot, rem;
} div_t;

typedef struct {
	long quot, rem;
} ldiv_t;

typedef struct {
	long long quot, rem;
} lldiv_t;

extern double atof(const char *);
extern int atoi(const char *);
extern long int atol(const char *);
extern long long int atoll(const char *);
extern double strtod(const char *restrict, char **restrict);
extern float strtof(const char *restrict, char **restrict);
extern long double strtold(const char *restrict, char **restrict);
extern long int strtol(const char *restrict, char **restrict, int);
extern long long int strtoll(const char *restrict, char **restrict, int);
extern unsigned long int strtoul(const char *restrict, char **restrict, int);
extern unsigned long long int strtoull(const char *restrict, char **restrict,
                                       int);
extern int rand(void);
extern void srand(unsigned int);
extern void *calloc(size_t, size_t);
extern void free(void *);
extern void *malloc(size_t);
extern void *realloc(void *, size_t);
extern void abort(void);
extern int atexit(void (*)(void));
extern void exit(int);
extern void _Exit(int);
extern char *getenv(const char *);
extern int system(const char *);
extern void *bsearch(const void *, const void *, size_t, size_t,
                     int (*)(const void *, const void *));
extern void qsort(void *, size_t, size_t, int (*)(const void *, const void *));
extern int abs(int);
extern long int labs(long int);
extern long long int llabs(long long int);
extern div_t div(int, int);
extern ldiv_t ldiv(long int, long int);
extern lldiv_t lldiv(long long int, long long int);

extern int mblen(const char *, size_t);
extern int mbtowc(wchar_t *restrict, const char *restrict, size_t);
extern int wctomb(char *, wchar_t);
extern size_t mbstowcs(wchar_t *restrict, const char *restrict, size_t);
extern size_t wcstombs(char *restrict, const wchar_t *restrict, size_t);

#endif
