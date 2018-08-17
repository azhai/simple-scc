
$(LIBEXEC)/cc1-i386-sysv: cc1-i386-sysv
	cp cc1-i386-sysv $@

OBJ-i386-sysv= $(OBJ)  target/i386-sysv/arch.o

cc1-i386-sysv: $(OBJ-i386-sysv) $(LIBSCC)/libscc.a
	$(CC) $(SCC_LDFLAGS) $(OBJ-i386-sysv) -lscc -o $@
