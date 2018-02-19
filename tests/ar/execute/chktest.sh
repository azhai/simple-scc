#!/bin/sh

ttyflags=`stty -g`
trap "stty $ttyflags;tabs -8" 0 2 3 15
stty tabs
tabs 40
ulimit -c 40
rm -f test.log

for i in test*.sh
do
	printf "%s\t" $i
	./$i && echo [OK] || echo [FAIL]
done
