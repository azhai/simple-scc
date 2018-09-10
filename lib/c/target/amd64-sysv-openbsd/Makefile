.POSIX:

PROJECTDIR = ../../../..
include $(PROJECTDIR)/rules.mk

SYS        = openbsd
ARCH       = amd64
ABI        = sysv
SYSERRTBL  = ../posix/netbsd.e
MORECFLAGS = -std=c99 -g -static -nostdinc -fno-stack-protector --freestanding
SYSOBJ     = raise.o signal.o _sigaction.o

include syscall.mk
include ../amd64-sysv/objlst.mk
include ../script/objlst.mk
include ../posix/objlst.mk
include ../script/common.mk
