
$(LIBEXEC)/cc2-qbe_amd64-sysv: cc2-qbe_amd64-sysv
	cp cc2-qbe_amd64-sysv $@

target/qbe/cgen.o: $(INCDIR)/$(STD)/cstd.h

OBJ-qbe_amd64-sysv = $(OBJ)  \
        target/qbe/cgen.o \
        target/qbe/optm.o \
        target/qbe/code.o \
        target/amd64-sysv/types.o

cc2-qbe_amd64-sysv: $(OBJ-qbe_amd64-sysv) $(LIBSCC)/libscc.a
	$(CC) $(SCC_LDFLAGS) $(OBJ-qbe_amd64-sysv) -lscc -o $@
