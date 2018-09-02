.POSIX:

PROJECTDIR = ../../../..
include $(PROJECTDIR)/rules.mk

SYSNAME    = amd64-sysv-openbsd
SCC_CFLAGS = -g -nopie -fno-stack-protector -static -nostdinc -I$(INCDIR) -I$(INCDIR)/bits/amd64-sysv/ -I.
SYSOBJ     = raise.o signal.o _sigaction.o

include syscall.mk
include ../script/objlst.mk
include ../posix/objlst.mk
include ../script/common.mk
