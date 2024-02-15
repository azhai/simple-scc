I386_OBJ =\
	$(OBJS)\
	target/x86/i386tbl.o\
	target/x86/i386.o\
	target/x86/ins.o\

target/x86/i386tbl.c: target/x86/ops.dat target/x86/opers.dat lexh
	./mktbl -f x86 -c i386

$(LIBEXEC)/scc/as-i386: $(I386_OBJ)
	$(CC) $(PROJ_LDFLAGS) $(I386_OBJ) $(PROJ_LDLIBS) -o $@
