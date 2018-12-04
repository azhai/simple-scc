OBJ-amd64-sysv= $(OBJS)  target/amd64-sysv/arch.o

$(LIBEXEC)/cc1-amd64-sysv: $(OBJ-amd64-sysv)
	$(CC) $(SCC_LDFLAGS) $(OBJ-amd64-sysv) -lscc -o $@
