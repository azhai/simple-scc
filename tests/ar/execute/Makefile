.POSIX:

PROJECTDIR=$$PWD/../../../
ROOTDIR=$(PROJECTDIR)/rootdir

all: tests

tests: master.a
	PATH=$(ROOTDIR)/bin:$$PATH ./chktest.sh

master.a:
	./mkmaster.sh

clean:
	rm -f *.a test.log
	rm -f file*
