AS = z80-unknown-coff-as
AR = ../../../bin/scc-ar
LD = ../../../bin/scc-ld
RL = ../../../bin/scc-ranlib
SZ = ../../../bin/scc-size

OBJS   = f1.o f2.o
OBJLIB = f3.o

all:
	@if $(AS) /dev/null 2>/dev/null;\
	then\
		$(MAKE) test;\
	fi


tests: a.out

a.out: $(OBJS) f.a
	$(LD) $(OBJS) f.a

f.a: $(OBJLIB)
	$(AR) -rv $@ $?

size:
	$(SZ) *.o *.a

clean:
	rm -f *.o *.a a.out core* *.dat
