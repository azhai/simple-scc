#!/bin/sh -x

set -e

TZ=UTC
tmp1=`mktemp`
tmp2=`mktemp`

#trap "rm -f $tmp1 $tmp2" 0 2 3

rm -f test.a

############################################################################
#create the test files
mkdir -p files
cd files

cat <<! >file1
This is the first file,
and it should go in the
first position in the archive.
!

cat <<! >file2
But this other one is the second one,
and it shouldn't go in the first position
because it should go in the second position.
!

cat <<! >file3
and at the end, this is the last file
that should go at the end of the file,
thus it should go in the third position.
!

cat <<! >file4
Ok, we can add a new file
and see what happens
!

touch -t 198001010000 file?

############################################################################
#generate the archive with the members in files
ar -qv ../test.a file1 file2
ar -qv ../test.a file3
cd ..

############################################################################
#verify that the output of ar -t is correct

ar -t test.a > $tmp1
cat <<! > $tmp2
file1
file2
file3
!

cmp $tmp1 $tmp2

############################################################################
#and now check that -tv doesn't break the code

ar -tv test.a >$tmp1
cat <<! >$tmp2
rw-r--r-- 1000/100	Tue Jan  1 00:00:00 1980 file1
rw-r--r-- 1000/100	Tue Jan  1 00:00:00 1980 file2
rw-r--r-- 1000/100	Tue Jan  1 00:00:00 1980 file3
!

cmp $tmp1 $tmp2

############################################################################
#extract the files
ar -xv test.a file1 file2 file3

cmp file1 files/file1
cmp file2 files/file2
cmp file3 files/file3

rm -f file1 file2 file3

############################################################################
#delete the 2nd file
ar -dv test.a file2

ar -tv test.a >$tmp1
cat <<! >$tmp2
rw-r--r-- 1000/100	Tue Jan  1 00:00:00 1980 file1
rw-r--r-- 1000/100	Tue Jan  1 00:00:00 1980 file3
!

cmp $tmp1 $tmp2
