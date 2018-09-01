#!/bin/sh

file=${1?' empty input file'}
tmp1=`mktemp`
tmp2=`mktemp`
trap "rm -f a.out $tmp1 $tmp2" 0 1 2 3 15
ulimit -c 0
rm -f test.log

while read i state
do
	rm -f a.out *.o

	(echo $i
	 ./cc.sh $CFLAGS $i.c
	 echo '/^output:$/+;/^end:$/-'w $tmp1 | ed -s $i.c
	 ./a.out > $tmp2
	 diff -u $tmp1 $tmp2) >> test.log 2>&1 &&
	printf '[PASS]' || printf '[FAIL]'
	printf '%s: %s\n' "$state" "$i"
done < $file
