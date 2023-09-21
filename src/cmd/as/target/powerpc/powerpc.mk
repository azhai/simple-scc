POWERPC = target/powerpc
POWERPC_OBJ =\
	$(OBJS)\
	$(POWERPC)/powerpctbl.o\
	$(POWERPC)/powerpc.o\
	$(POWERPC)/ins.o\

$(POWERPC)/powerpctbl.c: $(POWERPC)/ops.dat $(POWERPC)/opers.dat lexh
	./mktbl -f powerpc -c powerpc

$(LIBEXEC)/scc/as-powerpc: $(POWERPC_OBJ)
	$(CC) $(PROJ_LDFLAGS) $(POWERPC_OBJ) -lscc $(PROJ_LDLIBS) -o $@
