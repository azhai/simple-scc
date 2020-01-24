.POSIX:
PROJECTDIR =../../..
include $(PROJECTDIR)/scripts/rules.mk
include ../rules.mk

DIRS =\
	amd64\
	arm32\
	arm64\
	i386\
	ppc32\

all:
	+@cd $(ARCH) && $(MAKE)

dep clean:
	$(FORALL)
