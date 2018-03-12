#!/bin/sh

set -e

tmp1=`mktemp`
tmp2=`mktemp`
trap "rm -f a.out $tmp1 $tmp2" 0 2 3

z80-unknown-coff-as master.s
nm -u > $tmp1

cat <<! > $tmp2
                 U text6
!

diff $tmp1 $tmp2
