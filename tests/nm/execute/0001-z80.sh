#!/bin/sh

set -e

tmp1=`mktemp`
tmp2=`mktemp`
trap "rm -f a.out $tmp1 $tmp2" 0 2 3

z80-unknown-coff-as master.s
nm > $tmp1

cat <<! > $tmp2
0000000000000000 b .bss
0000000000000000 d .data
0000000000000000 t .text
0000000000000001 B averylongbss	
0000000000000001 D averylongdata
0000000000000001 T averylongtext
0000000000000000 B bss1
0000000000000002 b bss3
0000000000000000 D data1
0000000000000002 d data3
0000000000000000 T text1
0000000000000002 t text3
!

cmp $tmp1 $tmp2
