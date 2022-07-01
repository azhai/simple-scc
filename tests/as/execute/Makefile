.POSIX:

ROOT=../../../

all:
	@PATH=$(ROOT)/libexec/scc/:$(ROOT)/bin:$$PATH ./chktest.sh

tests:
	@$(MAKE) |\
	grep -v TODO |\
	grep FAIL >/dev/null &&\
	printf '[FAIL]' || printf '[PASS]'; printf '\tas/execute\n'

clean:
	rm -f test.log
