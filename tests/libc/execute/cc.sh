#!/bin/sh

set -e

for i
do
	case "$i" in
	-r)
		root=$2
		shift 2
		;;
	-a)
		abi=$2
		shift 2
		;;
	-s)
		sys=$2
		shift 2
		;;
	-o)
		out=$2
		shift 2
		;;
	-*)
		echo usage: cc.sh [-t target] file
		exit 1
		;;
	esac
done

sys=${sys:-`uname | tr 'A-Z' 'a-z'`}
abi=${abi:-amd64-sysv}
out=${out:-a.out}
root=${root:-$SCCPREFIX}
inc=$root/include/scc
arch_inc=$root/include/scc/bits/$abi
sys_inc=$root/include/scc/bits/$sys
lib=$root/lib/scc/${abi}-${sys}
obj=${1%.c}.o

includes="-nostdinc -I$inc -I$arch_inc -I$sys_inc"
flags="-std=c99 -g -w -fno-stack-protector --freestanding -static"

if ! gcc -nopie 2>&1 | grep unrecogn >/dev/null
then
	pie=-nopie
fi

gcc $flags $pie $includes -c $1
ld -g $pie -z nodefaultlib -static -L$lib $lib/crt.o $obj -lc -o $out
