SYSASM = $(SYS:.o=.s)
TARGET = $(LIBDIR)/$(SYSNAME)/libc.a

all: $(TARGET)

$(TARGET): $(OBJ) $(SYS)
	$(AR) $(ARFLAGS) $@ $?
	ranlib $@

clean:
	rm -f *.o *.a
	rm -f $(SYSASM)

$(SYSASM): syscall.lst
	../script/amd64-sysv.sh
