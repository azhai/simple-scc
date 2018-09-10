
raise.o: ../posix/raise.c ../../syscall.h
	$(CC) $(SCC_CFLAGS) ../posix/raise.c -c

signal.o: ../posix/signal.c ../../syscall.h
	$(CC) $(SCC_CFLAGS) ../posix/signal.c -c
