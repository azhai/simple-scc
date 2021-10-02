.POSIX:

PROJECTDIR = ../../..

DIRS =\
	amd64\
	arm64\
	arm\
	i386\
	ppc\
	netbsd\
	openbsd\
	darwin\
	dragonfly\
	linux\
	posix\

POSIX =\
	netbsd\
	openbsd\
	darwin\
	dragonfly\
	linux\

include $(PROJECTDIR)/scripts/rules.mk
include ../rules.mk

NODEP = 1

all: $(ARCH) $(SYS)

$(POSIX): posix
