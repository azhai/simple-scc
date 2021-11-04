.POSIX:

ROOT=../../..
SCCPREFIX=$(ROOT)
CC=$(ROOT)/bin/scc

all: tests

tests:
	@CC=$(CC) ./chktest.sh scc-tests.lst

clean:
	rm -f a.out *.s *.s *.as *.o *.ir *.qbe *core* test.log
