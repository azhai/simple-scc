#!/bin/sh

trap 'r=$?; rm -f $$.tmp; exit $r' EXIT HUP QUIT INT TERM

for i
do
	case $i in
	-o)
		out=$2
		shift 2
		;;
	--)
		shift
		break
		;;
	-*)
		echo usage: generrno.sh [-o output] file ...
		exit 1
		;;
	*)
		break
		;;
	esac
done

awk '
/^E/ && $2 > 0 {
	errno[$1] = $2
}

END {
	for (i in errno)
		print "#define", i, errno[i] | "sort -n -k3"
	close("sort -n -k3")
}' $@ > $$.tmp && mv $$.tmp ${out:-errno.h}
