#!/bin/sh

file=${1?' empty input file'}
tmp1=`mktemp`
tmp2=`mktemp`
trap "rm -f a.out *.o $tmp1 $tmp2; exit" 0 1 2 3 15
ulimit -c 0
rm -f test.log

while read i state
do
	state=${state:-"\t"}
	rm -f a.out *.o $tmp1 $tmp2

	(echo $i
	 ./cc.sh $CFLAGS -o $i $i.c
	 echo '/^output:$/+;/^end:$/-'w $tmp1 | ed -s $i.c
	 ./$i > $tmp2
	 diff -u $tmp1 $tmp2) >> test.log 2>&1 &&
	printf '[PASS]' || printf '[FAIL]'
	printf "$state\t%s\n" $i
done < $file
