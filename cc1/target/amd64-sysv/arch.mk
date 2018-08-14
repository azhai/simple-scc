all: cc1-amd64-sysv

OBJ-amd64-sysv= $(OBJ)  target/amd64-sysv/arch.o

cc1-amd64-sysv: $(OBJ-amd64-sysv) $(LIBDIR)/libscc.a
	$(CC) $(SCC_LDFLAGS) $(OBJ-amd64-sysv) -lscc -o $@
