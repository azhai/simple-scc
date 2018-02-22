.POSIX:

PROJECTDIR=$$PWD/../../../
ROOTDIR=$(PROJECTDIR)/rootdir

all: tests

tests:
	PATH=$(ROOTDIR)/bin:$$PATH ./chktest.sh

clean:
	rm -f *.a test.log
	rm -f file*
	rm -f *.dst
