#!/bin/sh

file=${1?' empty input file'}
trap "rm -f a.out" 0 1 2 3 15
ulimit -c 0
rm -f test.log

while read i state
do
	echo $i >>test.log
	rm -f a.out

	(./cc.sh $CFLAGS $i && ./a.out) 2>test.log &&
	printf '[PASS]' || printf '[FAIL]'
	printf '%s: %s\n' "$state" "$i"
done < $file
