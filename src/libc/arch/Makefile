.POSIX:
PROJECTDIR =../../..
include $(PROJECTDIR)/scripts/rules.mk
include ../rules.mk

DIRS = amd64 arm32 arm64

all:
	+@cd $(ARCH) && $(MAKE)

clean:
	$(FORALL)
