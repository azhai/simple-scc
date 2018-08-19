
AMD64_OBJ = $(OBJ) target/x86/amd64tbl.o target/x86/amd64.o target/x86/ins.o

target/x86/amd64tbl.c: gentbl.awk target/x86/x86.dat target/x86/rules.dat
	./gentbl.sh -f x86 -c amd64

$(LIBEXEC)/as-amd64: $(AMD64_OBJ)
	$(CC) $(SCC_LDFLAGS) $(AMD64_OBJ) -lscc -o $@
