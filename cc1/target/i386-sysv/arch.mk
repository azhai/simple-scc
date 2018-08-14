
all: cc1-i386-sysv

OBJ-i386-sysv= $(OBJ)  target/i386-sysv/arch.o

cc1-i386-sysv: $(OBJ-i386-sysv) $(LIBDIR)/libscc.a
	$(CC) $(SCC_LDFLAGS) $(OBJ-i386-sysv) -lscc -o $@
