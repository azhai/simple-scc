#!/bin/sh

set -e
trap "rm -f $$.tmp" 0 2 3

(sed '/^#deps/q' Makefile
for i in *.c
do
	sed -n '/#include "/ s/#include "\(.*\)"/'$i': \1/p' $i
done |
sort) > $$.tmp && mv $$.tmp Makefile
