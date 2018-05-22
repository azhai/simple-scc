#!/bin/sh

ttyflags=`stty -g`
trap "stty $ttyflags; tabs -8" 0 2 3 15
stty tabs
ulimit -c 0
rm -f test.log

for i in *.s
do
	cpu="${i%.s}"
	printf "Test: %s\n\n" $cpu >> test.log
	printf " Testing cpu=%s\t" $cpu
	./test.sh  $cpu && echo [OK] || echo [FAIL]
done
