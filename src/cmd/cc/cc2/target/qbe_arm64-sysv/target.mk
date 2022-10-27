OBJ-qbe_arm64-sysv = $(OBJS)  \
        target/qbe/cgen.o \
        target/qbe/optm.o \
        target/qbe/code.o \
        target/arm64-sysv/types.o \

$(LIBEXEC)/scc/cc2-qbe_arm64-sysv: $(LIBSCC) $(OBJ-qbe_arm64-sysv)
	$(CC) $(PROJ_LDFLAGS) $(OBJ-qbe_arm64-sysv) -lscc $(PROJ_LDLIBS) -o $@
