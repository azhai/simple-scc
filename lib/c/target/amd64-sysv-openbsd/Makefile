.POSIX:

PROJECTDIR = ../../../..
include $(PROJECTDIR)/rules.mk

SYSNAME    = amd64-sysv-netbsd
SCC_CFLAGS = -static -nostdinc -I$(INCDIR) -I$(INCDIR)/bits/amd64-sysv/ -I.
SYSOBJ     = raise.o signal.o _setcontext.o _sigaction.o _sigaction2.o

include syscall.mk
include ../script/objlst.mk
include ../posix/objlst.mk
include ../script/common.mk
