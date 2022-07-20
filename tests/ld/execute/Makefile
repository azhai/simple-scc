AS = z80-unknown-coff-as
AR = ../../../bin/scc-ar
LD = ../../../bin/scc-ld
RL = ../../../bin/scc-ranlib
SZ = ../../../bin/scc-size

OBJS   = f1.o f2.o
OBJLIB = f3.o

all: a.out

tests:
	@if $(AS) /dev/null 2>/dev/null;\
	then\
		$(MAKE) |\
		grep -v TODO |\
		grep FAIL >/dev/null &&\
		printf '[FAIL]' || printf '[PASS]';\
	else\
		printf '[SKIP]';\
	fi;\
	printf '\tld/execute\n'

a.out: $(OBJS) f.a
	$(LD) $(OBJS) f.a >/dev/null 2>&1 || true

f.a: $(OBJLIB)
	$(AR) -rv $@ $? >/dev/null 2>&1

size:
	$(SZ) *.o *.a

clean:
	rm -f *.o *.a a.out core* *.dat
