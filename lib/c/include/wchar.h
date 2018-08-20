#ifndef _WCHAR_H_
#define _WCHAR_H_


/* TODO: This is only a placeholder */
typedef long wchar_t;
typedef long wint_t;

extern size_t wcsnlen(const wchar_t *s, size_t maxlen);
extern wint_t putwc(wchar_t wc, FILE *fp);

#endif
