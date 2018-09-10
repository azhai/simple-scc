.POSIX:

PROJECTDIR = ../../../..
include $(PROJECTDIR)/rules.mk

SYS        = linux
ARCH       = amd64
ABI        = sysv
SYSERRTBL  = ../posix/linux.e
MORECFLAGS = -std=c99 -g -static -nostdinc
SYSOBJ     = raise.o signal.o

include syscall.mk
include ../amd64-sysv/objlst.mk
include ../script/objlst.mk
include ../posix/objlst.mk
include ../script/common.mk
