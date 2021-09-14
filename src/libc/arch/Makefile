.POSIX:

PROJECTDIR = ../../..

DIRS =\
	amd64\
	arm64\
	arm\
	darwin\
	dragonfly\
	i386\
	linux\
	netbsd\
	openbsd\
	posix\
	ppc\

BSD =\
	netbsd\
	openbsd\
	darwin\
	dragonfly\

include $(PROJECTDIR)/scripts/rules.mk
include ../rules.mk

NODEP = 1

all: $(ARCH) $(SYS)

linux: posix

$(BSD): bsd
