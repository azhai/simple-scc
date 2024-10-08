#ifndef _STDARG_H
#define _STDARG_H

#define _NEED_VA_LIST
#include <arch/cdefs.h>

typedef __va_list va_list;

#define va_start(ap, last) __builtin_va_start(ap, last)
#define va_end(ap) __builtin_va_end(ap)
#define va_copy(to, from) __builtin_va_copy(to, from)
#define va_arg(to, type) __builtin_va_arg(to, type)

#endif
