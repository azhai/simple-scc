SYSERRNO = $(INCDIR)/bits/$(SYS)/sys/errno.h

$(SYSERRNO): errno.lst
	../../generrno.sh -o $@ errno.lst

_sys_errlist.c: errno.lst $(SYSERRNO)
	../../generrstr.sh errno.lst
