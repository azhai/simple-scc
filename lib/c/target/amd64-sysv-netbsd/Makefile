.POSIX:

PROJECTDIR = ../../../..
include $(PROJECTDIR)/rules.mk

SYS        = netbsd
ARCH       = amd64
ABI        = sysv
SYSOBJ     = raise.o signal.o _sigaction.o _sigaction2.o _setcontext.o
SYSERRTBL  = ../posix/netbsd.e
MORECFLAGS = -std=c99 -g -static -nostdinc

include syscall.mk
include ../script/objlst.mk
include ../posix/objlst.mk
include ../script/common.mk
