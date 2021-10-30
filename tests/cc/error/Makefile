.POSIX:

ROOT=../../..
SCCPREFIX=$(ROOT)
CC=$(ROOT)/bin/scc-cc

all: tests

tests:
	CC=$(CC) SCCPREFIX=$(SCCPREFIX) ./chktest.sh scc-tests.lst

clean:
	rm -f *.as *.o *.ir *.qbe *core test.log
