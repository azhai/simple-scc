.POSIX:

PROJECTDIR = ../../../..
include $(PROJECTDIR)/rules.mk

SYSNAME    = amd64-sysv-netbsd
SCC_CFLAGS = -nostdinc -I$(INCDIR) -I$(INCDIR)/bits/amd64-sysv/
SYSOBJ = raise.o

include syscall.mk
include ../script/objlst.mk
include ../script/common.mk

raise.o: ../posix/raise.c ../../syscall.h
