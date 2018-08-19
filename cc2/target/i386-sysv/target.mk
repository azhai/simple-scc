
OBJ-i386-sysv = $(OBJ)  \
        target/i386-sysv/cgen.o \
        target/i386-sysv/optm.o \
        target/i386-sysv/code.o \
        target/i386-sysv/types.o

$(LIBEXEC)/cc2-i386-sysv: $(OBJ-i386-sysv)
	$(CC) $(SCC_LDFLAGS) $(OBJ-i386-sysv) -lscc -o $@
