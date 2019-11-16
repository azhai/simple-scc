.POSIX:

ROOT   = ../../..
CFLAGS =
CC     = SCCPREFIX=$(ROOT) ./cc.sh

.c:
	$(CC) $(CFLAGS) -o $@ $<

all: tests

tests:
	@CFLAGS='' SCCPREFIX=$(ROOT) ./chktest.sh libc-tests.lst

clean:
	rm -f *.o *core test.log `awk '{print $$1}' libc-tests.lst`
