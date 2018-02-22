#!/bin/sh

set -e

trap "rm -f file.a f1 f2 f3 $tmp1 $tmp2" 0 2 3

tmp1=`mktemp`
tmp2=`mktemp`

###########################################################################
#empty file list

rm -f file.a
ar -qv file.a file.a
