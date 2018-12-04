.POSIX:
PROJECTDIR =../..
include $(PROJECTDIR)/scripts/rules.mk

MORECFLAGS = -I$(INCDIR) -I$(INCDIR)/bits/$(SYS) -I$(INCDIR)/bits/$(ARCH)
TARGET = $(CRTDIR)/$(ARCH)-$(SYS)/libc.a
DIRS = arch\
       assert\
       ctype\
       locale\
       stdio\
       stdlib\
       string\
       time\

all: $(DIRS)
	+@$(MAKE) $(TARGET)

$(DIRS): FORCE
	+@cd $@ && $(MAKE)

objlst: FORCE
	./mklst $(TARGET)

$(TARGET): objlst
	xargs $(AR) $(ARFLAGS) $@ < objlst

clean:
	$(FORALL)
	rm -f objlst
