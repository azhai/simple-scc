OBJ-i386-sysv= $(OBJS)  target/i386-sysv/arch.o

$(LIBEXEC)/cc1-i386-sysv: $(OBJ-i386-sysv)
	$(CC) $(SCC_LDFLAGS) $(OBJ-i386-sysv) -lscc -o $@
