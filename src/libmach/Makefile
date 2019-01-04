.POSIX:
PROJECTDIR =../..
include $(PROJECTDIR)/scripts/rules.mk

TARGET = $(LIBDIR)/libmach.a
OBJS = object.o \
       archive.o \
       pack.o \
       unpack.o \
       artraverse.o \
       armember.o \
       coff32.o \

all: $(TARGET)

$(TARGET): $(OBJS)
	mklib -o $@ $?
