#!/bin/sh

root=`dirname $0`/..
SCCPREFIX=$root

export SCCPREFIX

exec $root/bin/scc-cc $@
