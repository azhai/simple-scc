
Z80_OBJ = $(OBJ) target/x80/z80tbl.o target/x80/z80.o target/x80/ins.o

target/x80/z80tbl.c: gentbl.awk target/x80/x80.dat target/x80/rules.dat
	./gentbl.sh -f x80 -c z80

$(LIBEXEC)/as-z80: $(OBJ) $(Z80_OBJ) $(LIBSCC)/libscc.a
	$(CC) $(SCC_LDFLAGS) $(Z80_OBJ) -lscc -o $@
