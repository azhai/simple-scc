all: cc2-z80-scc

target/z80-scc/code.o: $(INCDIR)/$(STD)/cstd.h

OBJ-z80-scc = $(OBJ)  \
        target/z80-scc/cgen.o \
        target/z80-scc/optm.o \
        target/z80-scc/code.o \
        target/z80-scc/types.o

cc2-z80-scc: $(OBJ-z80-scc) $(LIBSCC)/libscc.a
	$(CC) $(SCC_LDFLAGS) $(OBJ-z80-scc) -lscc -o $@
