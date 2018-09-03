.POSIX:

PROJECTDIR = ../../../..
include $(PROJECTDIR)/rules.mk

SYS        = netbsd
ARCH       = amd64
ABI        = sysv
SYSERRTBL  = ../posix/netbsd.e
MORECFLAGS = -std=c99 -g -static -nostdinc
SYSOBJ     = raise.o signal.o _sigaction.o _sigaction2.o _setcontext.o

include syscall.mk
include ../amd64-sysv/objlst.mk
include ../script/objlst.mk
include ../posix/objlst.mk
include ../script/common.mk
