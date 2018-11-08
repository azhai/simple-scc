#!/bin/sh

trap 'r=$?; rm -f $$.tmp; exit $r' EXIT HUP INT QUIT TERM

awk '
/^E/ && $2 > 0 {
	str = ""
	for (i = 3; i <= NF; i++)
		str = str " " $i
	sub(/^ /, "", str)
	errstr[$1] = str
	if ($2 > max)
		max = $2;
}

END {
	print "#include <errno.h>\n"
	print "char *_sys_errlist[] = {"
	for (i in errstr)
		printf "\t%-20.20s = \"%s\",\n", "[" i "]", errstr[i]
	print "};"
	print "int _sys_nerr =", $2 + 1 ";"
}' $@ > $$.tmp && mv $$.tmp _sys_errlist.c
