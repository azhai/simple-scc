#!/bin/sh

if test -z $SCCPREFIX
then
	SCCPREFIX=`dirname $0`/..
	export SCCPREFIX
fi

case $0 in
*cpp)
	exec $SCCPREFIX/bin/scc-cc -E $@
	;;
*scc)
	exec $SCCPREFIX/bin/scc-cc $@
	;;
esac
