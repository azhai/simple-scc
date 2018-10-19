.POSIX:

ROOT   = ../../../root
CFLAGS =
CC     = SCCPREFIX=$(ROOT) ./cc.sh

.c:
	$(CC) $(CFLAGS) -o $@ $<

all: tests

tests:
	@CFLAGS='' SCCPREFIX=$(ROOT) ./chktest.sh libc-tests.lst

clean:
	rm -f *.o *core a.out test.log
