#!/bin/sh

ttyflags=`stty -g`
trap "stty $ttyflags;tabs -8;rm -rf master.a file*" 0 2 3 15
stty tabs
tabs 40
ulimit -c 40
rm -f test.log
rm -rf file* master.a

uid=`id -u`
gid=`id -g`

cat <<EOF >master.a
!<arch>
file1           315532800   `printf "%-6d" $uid``printf "%-6d" $gid`100644  79        \`
This is the first file,
and it should go in the
first position in the archive.

file2           315532800   `printf "%-6d" $uid``printf "%-6d" $gid`100644  125       \`
But this other one is the second one,
and it shouldn't go in the first position
because it should go in the second position.

file3           315532800   `printf "%-6d" $uid``printf "%-6d" $gid`100644  118       \`
and at the end, this is the last file
that should go at the end of the file,
thus it should go in the third position.
EOF

for i in *-test*.sh
do
	printf "Test: %s\n\n" $i >> test.log
	printf "%s\t" $i
	./$i && echo [OK] || echo [FAIL]
done
