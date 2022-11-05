#ifdef _NEED_SIZET
#ifndef _SIZET
typedef unsigned size_t;
#define _SIZET
#endif
#undef _NEED_SIZET
#endif

#ifdef _NEED_PTRDIFFT
#ifndef _PTRDIFFT
typedef long ptrdiff_t;
#define _PTRDIFFT
#endif
#undef _NEED_PTRDIFFT
#endif

#ifdef _NEED_NULL
#ifndef NULL
#define NULL ((void *) 0)
#endif
#undef _NEED_NULL
#endif

#ifdef _NEED_WCHART
#ifndef _WCHART
typedef int wchar_t;
#define _WCHART
#endif
#undef _NEED_WCHART
#endif

#ifdef _NEED_WEOF
#ifndef WEOF
#define WEOF 0xFFFFFFFFU
#endif
#undef _NEED_WEOF
#endif

#ifdef _NEED_WINT
#ifndef _WINT
typedef unsigned wint_t;
#define _WINT
#endif
#undef _NEED_WINT
#endif

#ifdef _NEED_WCHARLIM
#ifndef _WCHARLIM
#define WCHAR_MAX 0x7FFFFFFF
#define WCHAR_MIN 0
#define _WCHARLIM
#endif
#undef _NEED_WCHARLIM
#endif
