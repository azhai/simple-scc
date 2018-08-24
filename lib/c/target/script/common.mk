SYSASM = $(SYSCALL:.o=.s)
TARGET = $(LIBDIR)/$(SYSNAME)/libc.a

all: $(TARGET)

$(TARGET): $(LIBOBJ) $(SYSOBJ) $(SYSCALL)
	$(AR) $(ARFLAGS) $@ $?
	ranlib $@

clean:
	rm -f *.o *.a
	rm -f $(SYSASM)
	rm -f $(TARGET)

$(SYSASM): syscall.lst
	../script/amd64-sysv.sh

dep:
	../script/syscall.sh
