.POSIX:

PROJECTDIR=$$PWD/../../../
ROOTDIR=$(PROJECTDIR)/rootdir

all: tests

tests:
	PATH=$(ROOTDIR)/bin:$$PATH ./chktest.sh

clean:
	rm -f test.log
