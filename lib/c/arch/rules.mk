SYSERRNO = $(INCDIR)/bits/$(SYS)/sys/errno.h

_sys_errlist.c: $(SYSERRNO)
	../../mkerrstr $(SYSERRNO)
