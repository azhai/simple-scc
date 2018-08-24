.POSIX:

PROJECTDIR = ../../../..
include $(PROJECTDIR)/rules.mk

SYSNAME    = amd64-sysv-linux
SCC_CFLAGS = -nostdinc -I$(INCDIR) -I$(INCDIR)/bits/amd64-sysv/
SYSOBJ = raise.o signal.o

include syscall.mk
include ../script/objlst.mk
include ../script/common.mk

raise.o: ../posix/raise.c ../../syscall.h
signal.o: ../posix/signal.c ../../syscall.h
