#!/bin/sh

exec >> test.log 2>&1


set -e

tmp1=`mktemp`
tmp2=`mktemp`
trap "rm -f file.a $tmp1 $tmp2" 0 2 3

############################################################################
#print 1st member

cp master.a file.a

ar -p file.a file1 > $tmp1

cat <<! > $tmp2
This is the first file,
and it should go in the
first position in the archive.
!

cmp $tmp1 $tmp2

############################################################################
#print 3rd member

ar -p file.a file3 > $tmp1

cat <<! > $tmp2
and at the end, this is the last file
that should go at the end of the file,
thus it should go in the third position.
!

cmp $tmp1 $tmp2

############################################################################
#print 2nd member with verbose

ar -pv file.a file2 >$tmp1

cat <<! > $tmp2

<file2>

But this other one is the second one,
and it shouldn't go in the first position
because it should go in the second position.
!

cmp $tmp1 $tmp2

############################################################################
#print all members

ar -p file.a file1 file2 file3 >$tmp1

cat <<! > $tmp2
This is the first file,
and it should go in the
first position in the archive.
But this other one is the second one,
and it shouldn't go in the first position
because it should go in the second position.
and at the end, this is the last file
that should go at the end of the file,
thus it should go in the third position.
!

cmp $tmp1 $tmp2

#and now with no members in command line

ar -p file.a > $tmp1

cmp $tmp1 $tmp2
