.POSIX:

PROJECTDIR=$$PWD/../../..
ROOTDIR=$(PROJECTDIR)/rootdir

all: tests

tests: master.s
	@PATH=$(ROOTDIR)/bin:$$PATH ./chktest.sh

clean:
	rm -f *.a *.out test.log
