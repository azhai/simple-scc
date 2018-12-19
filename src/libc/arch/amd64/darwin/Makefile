.POSIX:
PROJECTDIR =../../../../..
include $(PROJECTDIR)/scripts/rules.mk
include ../../../rules.mk

OBJS  = __Exit.o \
        __close.o \
        __getpid.o \
        __kill.o \
        __lseek.o \
        __open.o \
        __read.o \
        __write.o \
        __sigaction.o\
        _getheap.o \
        _tzone.o \
        getenv.o \
        raise.o \
        signal.o \
        time.o \
        _sys_errlist.o \

all: syscall
	$(MAKE) objs

objs: $(OBJS)

syscall: syscall.lst
	./gensys.sh syscall.lst
	touch syscall

clean:
	rm -f `awk '$$2 ~ /^__/ {print $$2".s"}' syscall.lst`
	rm -f syscall _sys_errlist.c
