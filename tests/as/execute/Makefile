.POSIX:

PROJECTDIR=$$PWD/../../../
ROOT=$(PROJECTDIR)/rootdir

all: tests

tests:
	PATH=$(ROOT)/bin:$$PATH ./chktest.sh

clean:
	rm -f test.log
