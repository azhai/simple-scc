.POSIX:

PROJECTDIR = ../..
DIRS =\
	assert\
	ctype\
	locale\
	stdio\
	stdlib\
	string\
	time\
	arch\

include $(PROJECTDIR)/scripts/rules.mk
include rules.mk
include objs/$(ARCH)-$(SYS).mk

NODEP = 1
TARGET = $(LIBC)

all: $(DIRS)
	$(MAKE) $(LIBC)

$(LIBC): $(OBJS)
	$(AR) $(PROJ_ARFLAGS) $(LIBC) $?
	$(RL) $(PROJ_RLFLAGS) $(LIBC)
