
raise.o: ../posix/raise.c
	$(CC) $(SCC_CFLAGS) ../posix/raise.c -c

signal.o: ../posix/signal.c
	$(CC) $(SCC_CFLAGS) ../posix/signal.c -c

getenv.o: ../posix/getenv.c
	$(CC) $(SCC_CFLAGS) ../posix/getenv.c -c

time.o: ../posix/time.c
	$(CC) $(SCC_CFLAGS) ../posix/time.c -c

_tzone.o: ../posix/_tzone.c ../../libc.h
	$(CC) $(SCC_CFLAGS) ../posix/_tzone.c -c
