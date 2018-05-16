AMD64_OBJ = $(OBJ) target/x86/amd64tbl.o target/x86/amd64.o target/x86/ins.o

target/x86/amd64tbl.c: gentbl.awk target/x86/x86.dat target/x86/rules.dat
	./gentbl.sh -f x86 -c amd64

as-amd64: $(AMD64_OBJ) $(LIBDIR)/libscc.a
	$(CC) $(SCC_LDFLAGS) $(AMD64_OBJ) -lscc -o $@

debug:
	echo $(AMD64_OBJ)
