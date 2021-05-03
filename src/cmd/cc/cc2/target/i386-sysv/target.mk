OBJ-i386-sysv = $(OBJS)  \
        target/i386-sysv/cgen.o \
        target/i386-sysv/optm.o \
        target/i386-sysv/code.o \
        target/i386-sysv/types.o

$(LIBEXEC)/scc/cc2-i386-sysv: $(LIBSCC) $(OBJ-i386-sysv)
	$(CC) $(PROJ_LDFLAGS) $(OBJ-i386-sysv) -lscc -o $@
