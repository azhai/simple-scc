.POSIX:
PROJECTDIR=../..
include $(PROJECTDIR)/scripts/rules.mk

OBJS   = crt-$(SYS).o crt-$(ABI).o
TARGET = $(CRTDIR)/$(ARCH)-$(SYS)/crt.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(LD) -r -o $@ $(OBJS)
