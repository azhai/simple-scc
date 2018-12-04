MORECFLAGS = -w -I$(INCDIR) -I$(INCDIR)/bits/$(SYS) -I$(INCDIR)/bits/$(ARCH)

SYSERRNO = $(INCDIR)/bits/$(SYS)/sys/errno.h

_sys_errlist.c: $(SYSERRNO)
	../../mkerrstr $(SYSERRNO)
