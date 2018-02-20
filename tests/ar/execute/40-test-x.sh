#!/bin/sh


exec >> test.log 2>&1
set -e

tmp1=`mktemp`
tmp2=`mktemp`
trap "rm -f file* $tmp1 $tmp2" 0 2 3


cp master.a file.a

############################################################################
#extract 1st member

ar -xv file.a file1

cat <<EOF > $tmp1
This is the first file,
and it should go in the
first position in the archive.
EOF

cmp file1 $tmp1

############################################################################
#extract 3rd member

ar -xv file.a file3

cat <<EOF > $tmp1
and at the end, this is the last file
that should go at the end of the file,
thus it should go in the third position.
EOF

cmp file3 $tmp1

############################################################################
#extract 3rd member

rm -f file1 file3
ar -xv file.a file1 file2 file3

cat <<EOF > $tmp1
This is the first file,
and it should go in the
first position in the archive.
But this other one is the second one,
and it shouldn't go in the first position
because it should go in the second position.
and at the end, this is the last file
that should go at the end of the file,
thus it should go in the third position.
EOF

cat file1 file2 file3 > $tmp2

cmp $tmp1 $tmp2

if test `ls file? | wc -l` -ne 3
then
	echo some error extracting files
	exit
fi


############################################################################
#extract all members

rm -f file1 file2 file3
ar -xv file.a

cat <<EOF > $tmp1
This is the first file,
and it should go in the
first position in the archive.
But this other one is the second one,
and it shouldn't go in the first position
because it should go in the second position.
and at the end, this is the last file
that should go at the end of the file,
thus it should go in the third position.
EOF

cat file1 file2 file3 > $tmp2

cmp $tmp1 $tmp2

if test `ls file? | wc -l` -ne 3
then
	echo some error extracting files
	exit
fi