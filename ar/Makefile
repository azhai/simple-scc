.POSIX:

PROJECTDIR = ..
LIBDIR = $(PROJECTDIR)/lib/scc
include $(PROJECTDIR)/rules.mk
include $(LIBDIR)/libdep.mk

OBJ       = main.o $(DRIVER)/driver.o
MOREFLAGS = -I$(DRIVER)

all: ar-$(DRIVER)
	mkdir -p $(PROJECTDIR)/rootdir/bin
	cp ar-$(DRIVER) $(PROJECTDIR)/rootdir/bin/ar

ar-$(DRIVER): $(OBJ) $(LIBDIR)/libscc.a
	$(CC) $(SCC_LDFLAGS) $(OBJ) -lscc -o $@

main.o: $(PROJECTDIR)/inc/scc.h $(PROJECTDIR)/inc/ar.h

$(DRIVER)/driver.o: $(DRIVER)/driver.h

$(LIBDIR)/libscc.a: $(LIB-OBJ)
	+cd $(LIBDIR) && $(MAKE)

main.o: $(DRIVER)/driver.h

dep:
clean:
	rm -f ar-* *.o $(DRIVER)/*.o

distclean: clean
