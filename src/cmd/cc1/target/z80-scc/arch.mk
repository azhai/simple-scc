OBJ-z80-scc= $(OBJS) target/z80-scc/arch.o

$(LIBEXEC)/cc1-z80-scc: $(OBJ-z80-scc)
	$(CC) $(SCC_LDFLAGS) $(OBJ-z80-scc) -lscc -o $@
