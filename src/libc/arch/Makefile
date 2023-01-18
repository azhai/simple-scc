.POSIX:

PROJECTDIR = ../../..

DIRS =\
	amd64\
	arm64\
	arm\
	i386\
	freebsd\
	ppc\
	netbsd\
	openbsd\
	darwin\
	dragonfly\
	linux\
	posix\

BSD =\
	netbsd\
	openbsd\
	darwin\
	dragonfly\

POSIX =\
	$(BSD)\
	linux\

include $(PROJECTDIR)/scripts/rules.mk
include ../rules.mk

NODEP = 1

all: $(ARCH) $(SYS)

$(POSIX): posix
$(BSD): bsd
