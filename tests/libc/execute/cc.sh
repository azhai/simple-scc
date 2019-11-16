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
abi=${abi:-i386}
out=${out:-a.out}
root=${root:-$SCCPREFIX}
inc=$root/include
arch_inc=$inc/bits/$abi
sys_inc=$inc/bits/$sys
lib=$root/lib/scc/${abi}-${sys}
obj=${1%.c}.o
cc=${CROSS_COMPILE}gcc
ld=${CROSS_COMPILE}ld

includes="-nostdinc -I$inc -I$arch_inc -I$sys_inc"
flags="-std=c99 -g -w -fno-stack-protector --freestanding -static"

if ! ${cc} -nopie 2>&1 | grep unrecogn >/dev/null
then
	pie=-no-pie
fi

${cc} $flags $pie $includes -c $1
${ld} -g -z nodefaultlib -static -L$lib $obj -lc -lcrt -o $out
