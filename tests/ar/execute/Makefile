.POSIX:

ROOT=../../..

all: master.a
	@TZ=UTC PATH=$(ROOT)/bin:$$PATH ./chktest.sh

master.a: mkmaster.sh
	@./mkmaster.sh

tests:
	@$(MAKE) |\
	grep -v TODO |\
	grep FAIL >/dev/null &&\
	printf '[FAIL]' || printf '[PASS]'; printf '\tar/execute\n'

clean:
	rm -f *.a test.log
	rm -f file*
