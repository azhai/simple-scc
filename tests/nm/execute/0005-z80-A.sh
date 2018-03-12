#!/bin/sh

set -e

tmp1=`mktemp`
tmp2=`mktemp`
trap "rm -f a.out f.out f.a $tmp1 $tmp2" 0 2 3

rm -f f.a
z80-unknown-coff-as master.s
ar -qv f.a a.out
cp a.out f.out
ar -qv f.a f.out
nm -A f.a a.out > $tmp1

cat <<! > $tmp2
f.a[a.out]: 0000000000000000 b .bss
f.a[a.out]: 0000000000000000 d .data
f.a[a.out]: 0000000000000000 t .text
f.a[a.out]: 0000000000000001 B averylongbss
f.a[a.out]: 0000000000000001 D averylongdata
f.a[a.out]: 0000000000000001 T averylongtext
f.a[a.out]: 0000000000000000 B bss1
f.a[a.out]: 0000000000000002 b bss3
f.a[a.out]: 000000000000000a C bss4
f.a[a.out]: 0000000000000012 C bss5
f.a[a.out]: 0000000000000000 D data1
f.a[a.out]: 0000000000000002 d data3
f.a[a.out]: 000000000000000a C data4
f.a[a.out]: 0000000000000012 C data5
f.a[a.out]: 0000000000000000 T text1
f.a[a.out]: 0000000000000002 t text3
f.a[a.out]: 000000000000000a C text4
f.a[a.out]: 0000000000000012 C text5
f.a[a.out]:                  U text6
f.a[f.out]: 0000000000000000 b .bss
f.a[f.out]: 0000000000000000 d .data
f.a[f.out]: 0000000000000000 t .text
f.a[f.out]: 0000000000000001 B averylongbss
f.a[f.out]: 0000000000000001 D averylongdata
f.a[f.out]: 0000000000000001 T averylongtext
f.a[f.out]: 0000000000000000 B bss1
f.a[f.out]: 0000000000000002 b bss3
f.a[f.out]: 000000000000000a C bss4
f.a[f.out]: 0000000000000012 C bss5
f.a[f.out]: 0000000000000000 D data1
f.a[f.out]: 0000000000000002 d data3
f.a[f.out]: 000000000000000a C data4
f.a[f.out]: 0000000000000012 C data5
f.a[f.out]: 0000000000000000 T text1
f.a[f.out]: 0000000000000002 t text3
f.a[f.out]: 000000000000000a C text4
f.a[f.out]: 0000000000000012 C text5
f.a[f.out]:                  U text6
a.out: 0000000000000000 b .bss
a.out: 0000000000000000 d .data
a.out: 0000000000000000 t .text
a.out: 0000000000000001 B averylongbss
a.out: 0000000000000001 D averylongdata
a.out: 0000000000000001 T averylongtext
a.out: 0000000000000000 B bss1
a.out: 0000000000000002 b bss3
a.out: 000000000000000a C bss4
a.out: 0000000000000012 C bss5
a.out: 0000000000000000 D data1
a.out: 0000000000000002 d data3
a.out: 000000000000000a C data4
a.out: 0000000000000012 C data5
a.out: 0000000000000000 T text1
a.out: 0000000000000002 t text3
a.out: 000000000000000a C text4
a.out: 0000000000000012 C text5
a.out:                  U text6
!

diff $tmp1 $tmp2
