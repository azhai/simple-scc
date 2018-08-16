
all: cc1-z80-scc

OBJ-z80-scc= $(OBJ)  target/z80-scc/arch.o

cc1-z80-scc: $(OBJ-z80-scc) $(LIBSCC)/libscc.a
	$(CC) $(SCC_LDFLAGS) $(OBJ-z80-scc) -lscc -o $@
