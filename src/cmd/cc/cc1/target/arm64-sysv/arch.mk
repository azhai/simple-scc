OBJ-arm64-sysv= $(OBJS)  target/arm64-sysv/arch.o

$(LIBEXEC)/cc1-arm64-sysv: $(OBJ-arm64-sysv)
	$(CC) $(SCC_LDFLAGS) $(OBJ-arm64-sysv) -lscc -o $@
