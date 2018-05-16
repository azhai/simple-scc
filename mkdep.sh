#!/bin/sh

set -e
trap "rm -f $$.tmp" 0 2 3

(sed '/^#deps/q' Makefile
for i in `find . -name '*.c'`
do
	file=`echo $i | sed -e 's,^./,,' -e 's/\.c$/.o/'`
	dir=`dirname $i | sed 's,/*$,,'`
	sed -n '/#include "/ s,#include "\(.*\)",'"$file: $dir"'/\1,p' $i
done |
sort) > $$.tmp && mv $$.tmp Makefile
