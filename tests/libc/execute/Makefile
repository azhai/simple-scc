.POSIX:

CC     = gcc-scc

.c:
	$(CC) $(CFLAGS) -o $@ $<

all: tests

tests:
	@CC=$(CC) ./chktest.sh libc-tests.lst

clean:
	rm -f *.o *core test.log `awk '{print $$1}' libc-tests.lst`
