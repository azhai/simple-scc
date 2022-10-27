CINCLUDES =\
	-I$(INCDIR)\
	-I$(INCDIR)/bits/$(SYS)\
	-I$(INCDIR)/bits/$(ARCH)\
	-I$(INCDIR)/bits/$(SYS)/$(ARCH)\

MORE_ARFLAGS = -u
MORE_CFLAGS = $(TOOL_LIBC_CFLAGS) $(NOPIE_CFLAGS)

SYSERRNO = $(INCDIR)/bits/$(SYS)/sys/errno.h

LIBC = $(LIBCDIR)/libc.a
CRT = $(LIBCDIR)/crt.o

# Rules

.SUFFIXES: .$O

_sys_errlist.c: $(SYSERRNO)
	../../mkerrstr $(SYSERRNO)

$(CRT): crt.$O
	cp crt.$O $@

clean: clean-libc

clean-libc: FORCE
	rm -f *.6? *.7 *.8 *.z *.q

.c.$O:
	$(CC) $(PROJ_CFLAGS) -o $@ -c $<

.s.$O:
	$(AS) $(PROJ_ASFLAGS) $< -o $@
