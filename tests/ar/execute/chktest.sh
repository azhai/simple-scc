#!/bin/sh

ttyflags=`stty -g`
trap "stty $ttyflags;tabs -8;rm -rf file*" 0 2 3 15
stty tabs
tabs 40
ulimit -c 40
rm -f test.log
rm -rf file*

for i in *-*.sh
do
	printf "Test: %s\n\n" $i >> test.log
	printf "%s\t" $i
	./$i >> test.log 2>&1 && echo [OK] || echo [FAIL]
done
