.POSIX:

PROJECTDIR = ..
include $(PROJECTDIR)/rules.mk
include $(LIBSCC)/libdep.mk

OBJ        = main.o $(DRIVER)/driver.o
MORECFLAGS = -I$(DRIVER)
STDCFLAGS  =

all: $(BINDIR)/ar

$(BINDIR)/ar: ar-$(DRIVER)
	cp ar-$(DRIVER) $@

ar-$(DRIVER): $(OBJ) $(LIBSCC)/libscc.a
	$(CC) $(SCC_LDFLAGS) $(OBJ) -lscc -o $@

dep:
	$(PROJECTDIR)/mkdep.sh

clean:
	rm -f ar-* *.o $(DRIVER)/*.o $(BINDIR)/ar

include deps.mk
