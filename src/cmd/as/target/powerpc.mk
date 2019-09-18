POWERPC_OBJ = $(OBJS) \
           target/powerpc/powerpctbl.o \
           target/powerpc/powerpc.o \
           target/powerpc/ins.o \

target/powerpc/powerpctbl.c: target/powerpc/powerpc.dat target/powerpc/rules.dat
	./mktbl -f powerpc -c powerpc

$(LIBEXEC)/as-powerpc: $(POWERPC_OBJ)
	$(CC) $(SCC_LDFLAGS) $(POWERPC_OBJ) -lscc -o $@
