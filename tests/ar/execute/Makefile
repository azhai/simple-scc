.POSIX:

PROJECTDIR=$$PWD/../../../
ROOT=$(PROJECTDIR)/root

all: tests

tests: master.a
	@TZ=UTC PATH=$(ROOT)/bin:$$PATH chktest.sh

master.a:
	./mkmaster.sh

clean:
	rm -f *.a test.log
	rm -f file*
