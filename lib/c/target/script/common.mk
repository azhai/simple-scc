SYSASM = $(SYS:.o=.s)

all: libc.a

libc.a: $(OBJ) $(SYS)
	$(AR) $(ARFLAGS) $@ $?
	ranlib $@

clean:
	rm -f *.o *.a
	rm -f $(SYSASM)

$(SYSASM): syscall.lst
	../script/amd64-sysv.sh
