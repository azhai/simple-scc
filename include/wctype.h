#ifndef _WCTYPE_H
#define _WCTYPE_H

#define _NEED_WEOF
#define _NEED_WINT
#include <sys/cdefs.h>
#include <arch/cdefs.h>

typedef const int *wctrans_t;
typedef unsigned long wctype_t;

extern int iswalnum(wint_t);
extern int iswalpha(wint_t);
extern int iswblank(wint_t);
extern int iswcntrl(wint_t);
extern int iswdigit(wint_t);
extern int iswgraph(wint_t);
extern int iswlower(wint_t);
extern int iswprint(wint_t);
extern int iswpunct(wint_t);
extern int iswspace(wint_t);
extern int iswupper(wint_t);
extern int iswxdigit(wint_t);
extern int iswctype(wint_t, wctype_t);
extern wctype_t wctype(const char *);
extern wint_t towlower(wint_t);
extern wint_t towupper(wint_t);
extern wint_t towctrans(wint_t, wctrans_t);
extern wctrans_t wctrans(const char *);

#endif
