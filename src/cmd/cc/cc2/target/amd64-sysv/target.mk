OBJ-amd64-sysv = $(OBJS)  \
        target/amd64-sysv/cgen.o \
        target/amd64-sysv/optm.o \
        target/amd64-sysv/code.o \
        target/amd64-sysv/types.o

$(LIBEXEC)/scc/cc2-amd64-sysv: $(LIBSCC) $(OBJ-amd64-sysv)
	$(CC) $(PROJ_LDFLAGS) $(OBJ-amd64-sysv) -lscc $(PROJ_LDLIBS) -o $@
