.POSIX:
PROJECTDIR =../..
include $(PROJECTDIR)/scripts/rules.mk
include rules.mk

TARGET = $(LIBCDIR)/libc.a
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

objlst:
	find . -name '*.o' > $@

$(TARGET): objlst FORCE
	mklib -o $@ `cat objlst`

clean:
	$(FORALL)
	rm -f objlst
