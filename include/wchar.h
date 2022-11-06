#ifndef _WCHAR_H
#define _WCHAR_H

#define _NEED_NULL
#define _NEED_SIZET
#define _NEED_WCHART
#define _NEED_WEOF
#define _NEED_WCHARLIM
#define _NEED_WINT
#include <sys/cdefs.h>
#include <arch/cdefs.h>

/*
 * We only support utf8 as multibyte encoding and
 * it does not have state.
 */
typedef int mbstate_t;

struct tm;

#ifdef _STDARG_H
extern int vswscanf(const wchar_t *restrict, const wchar_t *restrict, va_list);
extern int vwprintf(const wchar_t *restrict, va_list);
extern int vwscanf(const wchar_t *restrict format, va_list);
#endif

#ifdef _STDIO_H
extern int fwprintf(FILE *restrict, const wchar_t *restrict, ...);
extern int fwscanf(FILE *restrict, const wchar_t *restrict, ...);

#ifdef _STDARG_H
extern int vfwprintf(FILE *restrict, const wchar_t *restrict, va_list);
extern int vfwscanf(FILE *restrict, const wchar_t *restrict, va_list);
extern int vswprintf(wchar_t *restrict, size_t, const wchar_t *restrict, va_list);
#endif

extern wint_t fgetwc(FILE *);
extern wint_t fputwc(wchar_t c, FILE *);
extern wint_t getwc(FILE *);
extern wint_t putwc(wchar_t, FILE *);
extern int fwide(FILE *, int);
extern wint_t ungetwc(wint_t, FILE *);
extern wchar_t *fgetws(wchar_t *restrict, int, FILE *restrict);
extern int fputws(const wchar_t *restrict, FILE *restrict);
#endif

extern int swprintf(wchar_t *restrict, size_t, const wchar_t *restrict, ...);
extern int swscanf(const wchar_t *restrict, const wchar_t *restrict, ...);
extern int wprintf(const wchar_t *restrict, ...);
extern int wscanf(const wchar_t *restrict, ...);

extern wint_t getwchar(void);
extern wint_t putwchar(wchar_t);

extern double wcstod(const wchar_t *restrict, wchar_t **restrict);
extern float wcstof(const wchar_t *restrict, wchar_t **restrict);
extern long double wcstold(const wchar_t *restrict, wchar_t **restrict);

extern long int wcstol(const wchar_t *restrict, wchar_t **restrict, int);
extern long long int wcstoll(const wchar_t *restrict, wchar_t **restrict, int);
extern unsigned long int wcstoul(const wchar_t *restrict, wchar_t **restrict, int);
extern unsigned long long int wcstoull(const wchar_t *restrict, wchar_t **restrict, int);

extern wchar_t *wcscpy(wchar_t *restrict, const wchar_t *restrict);
extern wchar_t *wcsncpy(wchar_t *restrict, const wchar_t *restrict, size_t);

extern wchar_t *wmemcpy(wchar_t *restrict, const wchar_t *restrict, size_t);
extern wchar_t *wmemmove(wchar_t *, const wchar_t *, size_t);
extern wchar_t *wcscat(wchar_t *restrict, const wchar_t *restrict);
extern wchar_t *wcsncat(wchar_t *restrict, const wchar_t *restrict, size_t);
extern int wcscmp(const wchar_t *, const wchar_t *);
extern int wcscoll(const wchar_t *, const wchar_t *);
extern int wcsncmp(const wchar_t *, const wchar_t *, size_t);
extern size_t wcsxfrm(wchar_t *restrict, const wchar_t *restrict, size_t);
extern int wmemcmp(const wchar_t *, const wchar_t *, size_t);
extern wchar_t *wcschr(const wchar_t *, wchar_t);
extern size_t wcscspn(const wchar_t *, const wchar_t *);
extern wchar_t *wcspbrk(const wchar_t *, const wchar_t *);
extern wchar_t *wcsrchr(const wchar_t *, wchar_t);
extern size_t wcsspn(const wchar_t *, const wchar_t *);
extern wchar_t *wcsstr(const wchar_t *, const wchar_t *);
extern wchar_t *wcstok(wchar_t *restrict, const wchar_t *restrict, wchar_t **restrict);
extern wchar_t *wmemchr(const wchar_t *, wchar_t, size_t);
extern size_t wcslen(const wchar_t *);
extern wchar_t *wmemset(wchar_t *, wchar_t, size_t);
extern size_t wcsftime(wchar_t *restrict, size_t, const wchar_t *restrict, const struct tm *restrict);
extern wint_t btowc(int);
extern int wctob(wint_t);
extern int mbsinit(const mbstate_t *);
extern size_t mbrlen(const char *restrict, size_t, mbstate_t *restrict);
extern size_t mbrtowc(wchar_t *restrict, const char *restrict, size_t, mbstate_t *restrict);
extern size_t wcrtomb(char *restrict, wchar_t, mbstate_t *restrict);
extern size_t mbsrtowcs(wchar_t *restrict, const char **restrict, size_t, mbstate_t *restrict);
extern size_t wcsrtombs(char *restrict, const wchar_t **restrict, size_t, mbstate_t *restrict);
extern int wcwidth(wchar_t);
#endif
