.POSIX:
PROJECTDIR=../../..
include $(PROJECTDIR)/scripts/rules.mk

OBJS = crt-$(SYS).o
TARGET = $(LIBCDIR)/libcrt.a

all: $(TARGET)

$(TARGET): $(OBJS)
	mklib -o $@ $?
