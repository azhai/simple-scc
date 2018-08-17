
$(LIBEXEC)/as-i286: as-i286
	cp as-i286 $@

I286_OBJ = $(OBJ) target/x86/i286tbl.o target/x86/i286.o target/x86/ins.o

target/x86/i286tbl.c: gentbl.awk target/x86/x86.dat target/x86/rules.dat
	./gentbl.sh -f x86 -c i286

as-i286: $(I286_OBJ) $(LIBSCC)/libscc.a
	$(CC) $(SCC_LDFLAGS) $(I286_OBJ) -lscc -o $@
