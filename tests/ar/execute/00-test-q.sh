#!/bin/sh

exec >> test.log 2>&1

set -e

trap "rm -f file.a;\
      rm -f *.tst" 0 2 3

###########################################################################
#Generate a bunch of files that we can use to test -q

for i in `awk 'BEGIN {for (i=0; i <999; ++i) print i}'`
do
	echo $i > $i.tst
done


###########################################################################
#Append all the generated files one by one

rm -f file.a
ls | grep .tst | sort -n | xargs -n 1 ar -qv file.a

ar -t file.a |
tee -a test.log |
awk '$0 != NR-1 ".tst" {
	printf "bad line %d:%s\n", NR, $0
	exit 1
}'

###########################################################################
#Appenp all the generated files 10 by 10

rm -f file.a
ls | grep .tst | sort -n | xargs -n 10 ar -qv file.a

ar -t file.a |
tee -a test.log |
awk '$0 != NR-1 ".tst" {
	printf "bad line %d:%s\n", NR, $0
	exit 1
}'


###########################################################################
#Test special cases

#empty file list
rm -f file.a
ar -qv file.a
if ! test -f file.a
then
	echo "ar -q didn't generated empty archive" >&2
	exit 1
fi

#recursive inclusion
rm -f file.a
ar -qv file.a file.a


#missed file
ar -qv file.a badfile.a || true
if ar -qv file.a badfile.a
then
	echo "ar -q failed to detect missed file" >&2
	exit 1
fi
