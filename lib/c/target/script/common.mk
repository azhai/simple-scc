SYSNAME  = $(ARCH)-$(ABI)-$(SYS)
SYSASM   = $(SYSCALL:.o=.s)
TARGET   = $(LIBDIR)/$(SYSNAME)/libc.a
INCLUDE  = -I$(INCDIR) \
           -I$(INCDIR)/bits/$(ARCH)-$(ABI) \
           -I$(INCDIR)/bits/$(SYS) \
           -I.
SYSERRNO = $(INCDIR)/bits/$(SYS)/sys/errno.h
OBJ      = $(LIBOBJ) $(SYSOBJ) $(SYSCALL) $(ARCHOBJ)

SCC_CFLAGS = $(MORECFLAGS) $(INCLUDE)

all: $(TARGET)

$(OBJ): $(SYSERRNO)

$(SYSERRNO): $(SYSERRTBL)
	trap "rm -f $$$$.tmp" 0 2 3 4; \
	../script/generrno.sh $(SYSERRTBL) > $$$$.tmp && mv $$$$.tmp $@

_sys_errlist.c: $(SYSERRTBL) $(SYSERRNO)
	trap "rm -f $$$$.tmp" 0 2 3 4; \
	../script/generrstr.sh $(SYSERRTBL) > $$$$.tmp && mv $$$$.tmp $@

$(TARGET): $(OBJ)
	$(AR) $(ARFLAGS) $@ $?
	ranlib $@

clean:
	rm -f *.o *.a _sys_errlist.c $(SYSERRNO)
	rm -f $(SYSASM)
	rm -f $(TARGET)

$(SYSASM): syscall.lst
	../script/amd64-sysv.sh

dep:
	../script/syscall.sh
