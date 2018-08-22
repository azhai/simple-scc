.POSIX:

PROJECTDIR = ../../../..
SYSNAME    = amd64-sysv-linux

include $(PROJECTDIR)/rules.mk
include ../script/objlst.mk
include syscall.mk
include ../script/common.mk

SCC_CFLAGS = -nostdinc -I$(INCDIR) -I$(INCDIR)/bits/amd64-sysv/

dep:
	../script/syscall.sh
