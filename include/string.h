#ifndef _STRING_H
#define _STRING_H

#define _NEED_SIZET
#define _NEED_NULL
#include <sys/cdefs.h>
#include <arch/cdefs.h>
#include <arch/string.h>

extern void *memcpy(void *restrict, const void *restrict, size_t);
extern void *memmove(void *, const void *, size_t);
extern char *strcpy(char *restrict, const char *restrict);
extern char *strncpy(char *restrict, const char *restrict, size_t);
extern char *strcat(char *restrict, const char *restrict);
extern char *strncat(char *restrict, const char *restrict, size_t);
extern int memcmp(const void *, const void *, size_t);
extern int strcmp(const char *, const char *);
extern int strcoll(const char *, const char *);
extern int strncmp(const char *, const char *, size_t);
extern size_t strxfrm(char *restrict, const char *restrict, size_t);
extern void *memchr(const void *, int, size_t);
extern char *strchr(const char *, int);
extern size_t strcspn(const char *, const char *);
extern char *strpbrk(const char *, const char *);
extern char *strrchr(const char *, int);
extern size_t strspn(const char *, const char *);
extern char *strstr(const char *, const char *);
extern char *strtok(char *restrict, const char *restrict);
extern void *memset(void *, int, size_t);
extern char *strerror(int);
extern size_t strlen(const char *);

#endif
