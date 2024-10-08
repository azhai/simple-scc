#define INT8_MAX  0x7F
#define INT8_MIN  (-INT8_MAX - 1)
#define UINT8_MAX 0xFFU

#define INT16_MAX  0x7FFF
#define INT16_MIN  (-INT16_MAX - 1)
#define UINT16_MAX 0xFFFF

#define INT32_MAX  0x7FFFFFFF
#define INT32_MIN  (-INT32_MAX - 1)
#define UINT32_MAX 0xFFFFFFFF

#define INT64_MAX  0x7FFFFFFFFFFFFFFF
#define INT64_MIN  (-INT64_MAX - 1)
#define UINT64_MAX 0xFFFFFFFFFFFFFFFF

#define INT_LEAST8_MIN  INT8_MIN
#define INT_LEAST8_MAX  INT8_MAX
#define UINT_LEAST8_MAX UINT8_MAX

#define INT_LEAST16_MIN  INT16_MIN
#define INT_LEAST16_MAX  INT16_MAX
#define UINT_LEAST16_MAX UINT16_MAX

#define INT_LEAST32_MIN  INT32_MIN
#define INT_LEAST32_MAX  INT32_MAX
#define UINT_LEAST32_MAX UINT32_MAX

#define INT_LEAST64_MIN  INT64_MIN
#define INT_LEAST64_MAX  INT64_MAX
#define UINT_LEAST64_MAX UINT64_MAX

#define INT_FAST8_MIN  INT8_MIN
#define INT_FAST8_MAX  INT8_MAX
#define UINT_FAST8_MAX UINT8_MAX

#define INT_FAST16_MIN  INT32_MIN
#define INT_FAST16_MAX  INT32_MAX
#define UINT_FAST16_MAX UINT32_MAX

#define INT_FAST32_MIN  INT32_MIN
#define INT_FAST32_MAX  INT32_MAX
#define UINT_FAST32_MAX UINT32_MAX

#define INT_FAST64_MIN  INT64_MIN
#define INT_FAST64_MAX  INT64_MAX
#define UINT_FAST64_MAX UINT64_MAX

#define INTPTR_MIN  INT64_MIN
#define INTPTR_MAX  INT64_MAX
#define UINTPTR_MAX UINT64_MAX

#define INTMAX_MIN  INT64_MIN
#define INTMAX_MAX  INT64_MAX
#define UINTMAX_MAX UINT64_MAX

#define PTRDIFF_MIN INT64_MIN
#define PTRDIFF_MAX INT64_MAX

#define SIG_ATOMIC_MIN INT32_MIN
#define SIG_ATOMIC_MAX INT32_MAX

#define SIZE_MAX UINT64_MAX

#define INT8_C(x)  x
#define INT16_C(x) x
#define INT32_C(x) x
#define INT64_C(x) x ## LL

#define UINT8_C(x)  x
#define UINT16_C(x) x
#define UINT32_C(x) x ## U
#define UINT64_C(x) x ## ULL

#define INTMAX_C(x)  x ## LL
#define UINTMAX_C(x) x ## ULL

typedef signed char int8_t;
typedef short int16_t;
typedef int int32_t;
typedef long long int64_t;

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;

typedef signed char int_least8_t;
typedef short int_least16_t;
typedef int int_least32_t;
typedef long long int_least64_t;

typedef unsigned char uint_least8_t;
typedef unsigned short uint_least16_t;
typedef unsigned int uint_least32_t;
typedef unsigned long long uint_least64_t;

typedef signed char int_fast8_t;
typedef int int_fast16_t;
typedef int int_fast32_t;
typedef long long int_fast64_t;

typedef unsigned char uint_fast8_t;
typedef unsigned int uint_fast16_t;
typedef unsigned int uint_fast32_t;
typedef unsigned long long uint_fast64_t;

typedef long intptr_t;
typedef unsigned long uintptr_t;

typedef long intmax_t;
typedef unsigned long uintmax_t;
