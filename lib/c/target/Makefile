
PROJECTDIR = ../../..

include $(PROJECTDIR)/rules.mk

DIRS = amd64-sysv-linux-elf \
       amd64-sysv-openbsd-elf

#       i386-sysv-linux-elf \
#       z80-scc-none-none

dep:
	./objlst.sh

all clean distclean:
	$(FORALL)
