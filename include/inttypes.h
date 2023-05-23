#ifndef _INTTYPES_H_
#define _INTTYPES_H_

#include <stdint.h>
#include <arch/inttypes.h>

typedef struct {
        intmax_t quot, rem;
} imaxdiv_t;

extern intmax_t imaxabs(intmax_t);
extern imaxdiv_t imaxdiv(intmax_t, intmax_t);
extern intmax_t strtoimax(const char * restrict, char ** restrict, int);
extern uintmax_t strtoumax(const char * restrict, char ** restrict, int);

#ifdef _NEED_WCHART
extern intmax_t wcstoimax(const wchar_t * restrict, wchar_t ** restrict, int);
extern uintmax_t wcstoumax(const wchar_t * restrict, wchar_t ** restrict, int);
#endif

#endif
