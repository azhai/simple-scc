
PROJECTDIR = ../../..

include $(PROJECTDIR)/rules.mk

DIRS = amd64-sysv-linux \
       amd64-sysv-openbsd

#       i386-sysv-linux-elf \
#       z80-scc-none-none

dep:
	./script/objlst.sh

all clean distclean:
	$(FORALL)
