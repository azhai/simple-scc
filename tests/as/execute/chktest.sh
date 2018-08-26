#!/bin/sh

ulimit -c 0
rm -f test.log

for i in *.s
do
	cpu="${i%.s}"
	printf "Test: %s\n\n" $cpu >> test.log
	./test.sh  $cpu && printf '[PASS]: ' || printf '[FAIL]: '
	printf 'testing cpu=%s\n' "$cpu"
done
