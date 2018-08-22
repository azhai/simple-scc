.POSIX:

PROJECTDIR=$$PWD/../../../
ROOT=$(PROJECTDIR)/rootdir

all: tests

tests: master.a
	PATH=$(ROOT)/bin:$$PATH ./chktest.sh

master.a:
	./mkmaster.sh

clean:
	rm -f *.a test.log
	rm -f file*
