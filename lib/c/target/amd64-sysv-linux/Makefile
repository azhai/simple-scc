.POSIX:

PROJECTDIR = ../../../..
include $(PROJECTDIR)/rules.mk

SYS        = linux
ARCH       = amd64
ABI        = sysv
SYSOBJ     = raise.o signal.o
SYSERRTBL  = ../posix/linux.e
MORECFLAGS = -std=c99 -g -static -nostdinc

include syscall.mk
include ../script/objlst.mk
include ../posix/objlst.mk
include ../script/common.mk
