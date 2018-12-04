INCLUDE    = -I$(INCDIR) -I$(INCDIR)/bits/$(SYS) -I$(INCDIR)/bits/$(ARCH)
MORECFLAGS = -w

SYSERRNO = $(INCDIR)/bits/$(SYS)/sys/errno.h

_sys_errlist.c: $(SYSERRNO)
	../../mkerrstr $(SYSERRNO)
