#ifndef _ERRNO_H
#define _ERRNO_H

#include <sys/errno.h>

extern int errno;
extern char *_sys_errlist[];
extern int _sys_nerr;

#endif
