.POSIX:

PROJECTDIR = ..
include $(PROJECTDIR)/rules.mk
include $(LIBSCC)/libdep.mk

OBJ       = main.o $(DRIVER)/driver.o
MOREFLAGS = -I$(DRIVER)

all: ar-$(DRIVER)
	cp ar-$(DRIVER) $(PROJECTDIR)/rootdir/bin/ar

ar-$(DRIVER): $(OBJ) $(LIBSCC)/libscc.a
	$(CC) $(SCC_LDFLAGS) $(OBJ) -lscc -o $@

dep:
	$(PROJECTDIR)/mkdep.sh
clean:
	rm -f ar-* *.o $(DRIVER)/*.o

include deps.mk
