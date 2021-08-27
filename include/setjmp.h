#ifndef _SETJMP_H
#define _SETJMP_H

#include <arch/setjmp.h>

extern int setjmp(jmp_buf);
extern void longjmp(jmp_buf, int);

#define setjmp setjmp

#endif
