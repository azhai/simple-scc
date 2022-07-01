.POSIX:

ROOT=../../..
SCCPREFIX=$(ROOT)
CC=$(ROOT)/bin/scc

all:
	@CC=$(CC) ./chktest.sh scc-tests.lst

tests:
	@$(MAKE) |\
	grep -v TODO |\
	grep FAIL >/dev/null &&\
	printf '[FAIL]' || printf '[PASS]'; printf '\tcc/error\n'

clean:
	rm -f *.as *.o *.ir *.qbe *core test.log
