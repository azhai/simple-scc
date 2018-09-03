#!/bin/sh

file=${1?' empty input file'}
err=/tmp/$$.err
chk=/tmp/$$.chk

trap "rm -f a.out *.o $chk $err; exit" 0 1 2 3 15
ulimit -c 0
rm -f test.log

while read i state
do
	echo $i >> test.log

	scc $CFLAGS -w -c $i 2> $err
	echo "/^PATTERN/+;/^\./-w $chk" | ed -s $i
	diff -c $chk $err >> test.log  && printf '[PASS]' || echo '[FAIL]'
	printf "%s: %s\n" "$state" "$i"
	rm -f *.o
done < $file
