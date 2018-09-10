.POSIX:

PROJECTDIR = ../..
include $(PROJECTDIR)/rules.mk

DIRS = amd64-sysv-linux amd64-sysv-openbsd amd64-sysv-netbsd

all: $(DIRS)

$(DIRS): FORCE
	@cd $@ && $(MAKE)

clean:
	$(FORALL)
