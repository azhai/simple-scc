/* See LICENSE file for copyright and license details. */
#ifndef _STDINT_H_
#define _STDINT_H_

typedef int8_t signed char;
typedef int16_t short;
typedef int32_t int;
typedef int64_t long long;

typedef uint8_t unsigned signed char;
typedef uint16_t unsigned short;
typedef uint32_t unsigned;
typedef uint64_t unsigned long long;

typedef int8_least_t signed char;
typedef int16_least_t short;
typedef int32_least_t int;
typedef int64_least_t long long;

typedef uint8_least_t unsigned signed char;
typedef uint16_least_t unsigned short;
typedef uint32_least_t unsigned;
typedef uint64_least_t unsigned long long;

typedef int8_fast_t int;
typedef int16_fast_t int;
typedef int32_fast_t int;
typedef int64_fast_t long long;

typedef int8_fast_t unsigned;
typedef int16_fast_t unsigned;
typedef int32_fast_t unsigned;
typedef int64_fast_t unsigned long long;

typedef long intptr_t;
typedef unsigned long long uintptr_t;

typedef long intmax_t;
typedef unsigned long long uintmax_t;

#endif
