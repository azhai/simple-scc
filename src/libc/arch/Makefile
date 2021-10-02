.POSIX:

PROJECTDIR = ../../..

DIRS =\
	$(ARCHS)\
	$(SYSTEMS)\
	posix\

ARCHS =\
	amd64\
	arm64\
	arm\
	i386\
	ppc\

POSIX =\
	netbsd\
	openbsd\
	darwin\
	dragonfly\
	linux\

SYSTEM =\
	$(POSIX)\

include $(PROJECTDIR)/scripts/rules.mk
include ../rules.mk

NODEP = 1

all: $(ARCH) $(SYS)

$(POSIX): posix
