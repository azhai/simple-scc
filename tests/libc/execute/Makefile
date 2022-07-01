.POSIX:

CC     = gcc-scc

.c:
	$(CC) $(CFLAGS) -o $@ $<

all:
	CC=$(CC) ./chktest.sh libc-tests.lst

tests:
	@$(MAKE) |\
	grep -v TODO |\
	grep FAIL >/dev/null &&\
	printf '[FAIL]' || printf '[PASS]'; printf '\tlibc/execute\n'

clean:
	rm -f *.o *core test.log `awk '{print $$1}' libc-tests.lst`
