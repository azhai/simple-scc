
PROJECTDIR = ../../..

include $(PROJECTDIR)/rules.mk

DIRS = amd64-sysv-linux \
       amd64-sysv-openbsd \
       amd64-sysv-netbsd \

all clean distclean:
	$(FORALL)

dep:
	./script/objlst.sh
	$(FORALL)
