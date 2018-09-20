.POSIX:

ROOT=../../../root

all: tests

tests:
	@CFLAGS='' SCCPREFIX=$(ROOT) PATH=$(ROOT)/bin:$$PATH ./chktest.sh scc-tests.lst

clean:
	rm -f *.as *.o *.ir *.qbe *core test.log
