.POSIX:

ROOT=../../..

AS = z80-unknown-coff-as
OUT = z80.out

all: $(OUT)
	@PATH=$(ROOT)/bin:$$PATH:. chktest.sh

tests:
	@if $(AS) /dev/null 2>/dev/null;\
	then\
		$(MAKE) | \
		grep -v TODO |\
		grep FAIL >/dev/null &&\
		printf '[FAIL]' || printf '[PASS]';\
	else\
		printf '[SKIP]';\
	fi;\
	printf '\tsize/execute\n'

z80.out: master.s
	z80-unknown-coff-as -o $@ master.s

clean:
	rm -f *.a *.out test.log
