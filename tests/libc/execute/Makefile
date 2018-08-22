.POSIX:

ROOT=../../../rootdir

all: tests

tests:
	@CFLAGS='' SCCPREFIX=$(ROOT) ./chktest.sh libc-tests.lst

clean:
	rm -f *.o *core a.out test.log

