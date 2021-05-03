AMD64_OBJ =\
	$(OBJS)\
	target/x86/amd64tbl.o\
	target/x86/amd64.o\
	target/x86/ins.o\

target/x86/amd64tbl.c: target/x86/ops.dat target/x86/opers.dat
	./mktbl -f x86 -c amd64

$(LIBEXEC)/scc/as-amd64: $(AMD64_OBJ)
	$(CC) $(PROJ_LDFLAGS) $(AMD64_OBJ) -lscc -o $@
