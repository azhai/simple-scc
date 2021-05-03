OBJ-qbe_amd64-sysv = $(OBJS)  \
        target/qbe/cgen.o \
        target/qbe/optm.o \
        target/qbe/code.o \
        target/amd64-sysv/types.o

$(LIBEXEC)/scc/cc2-qbe_amd64-sysv: $(LIBSCC) $(OBJ-qbe_amd64-sysv)
	$(CC) $(PROJ_LDFLAGS) $(OBJ-qbe_amd64-sysv) -lscc -o $@
