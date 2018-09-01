#!/bin/sh

file=${1?' empty input file'}
tmp=`mktemp`
#trap "rm -f a.out $tmp" 0 1 2 3 15
ulimit -c 0
rm -f test.log

while read i state
do
	rm -f a.out

	(echo
	 echo $i
	 ./cc.sh $CFLAGS $i.c &&
	 ./a.out > $tmp &&
	 cat $tmp
	 diff -u $tmp $i.txt) > test.log 2>&1 &&
	printf '[PASS]' || printf '[FAIL]'
	printf '%s: %s\n' "$state" "$i"
done < $file
