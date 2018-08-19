
OBJ-amd64-sysv = $(OBJ)  \
        target/amd64-sysv/cgen.o \
        target/amd64-sysv/optm.o \
        target/amd64-sysv/code.o \
        target/amd64-sysv/types.o

$(LIBEXEC)/cc2-amd64-sysv: $(OBJ-amd64-sysv) $(LIBSCC)/libscc.a
	$(CC) $(SCC_LDFLAGS) $(OBJ-amd64-sysv) -lscc -o $@
