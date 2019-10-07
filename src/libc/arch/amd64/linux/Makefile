.POSIX:
PROJECTDIR =../../../../..
include $(PROJECTDIR)/scripts/rules.mk
include ../../../rules.mk

OBJS  = _Exit.o \
        _close.o \
        _getpid.o \
        _kill.o \
        _lseek.o \
        _open.o \
        _sigaction.o \
        _read.o \
        _write.o \
        _brk.o \
        _getheap.o \
        _cerrno.o \
        _sigaction.o \
        _tzone.o \
        getenv.o \
        raise.o \
        signal.o \
        time.o \
        _sys_errlist.o \
        crt.o \

all: syscall
	$(MAKE) objs

objs: $(OBJS)

crt.o: ../crt-posix.s

syscall: syscall.lst
	gensys.sh syscall.lst
	touch syscall

clean:
	rm -f `awk '/[0-9]*	_/ {print $$2".s"}' syscall.lst`
	rm -f syscall _sys_errlist.c
