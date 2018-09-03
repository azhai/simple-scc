.POSIX:

PROJECTDIR = ../../../..
include $(PROJECTDIR)/rules.mk

SYS        = openbsd
ARCH       = amd64
ABI        = sysv
SYSOBJ     = raise.o signal.o _sigaction.o
SYSERRTBL  = ../posix/netbsd.e
MORECFLAGS = -std=c99 -g -static -nostdinc -fno-stack-protector --freestanding

include syscall.mk
include ../script/objlst.mk
include ../posix/objlst.mk
include ../script/common.mk
