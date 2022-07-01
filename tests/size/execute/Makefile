.POSIX:

ROOT=../../..

AS = z80-unknown-coff-as
OUT = z80.out

all:
	@if $(AS) /dev/null 2>/dev/null;\
	then\
		$(MAKE) tests;\
	fi

tests: $(OUT)
	@PATH=$(ROOT)/bin:$$PATH:. chktest.sh;\

z80.out: master.s
	z80-unknown-coff-as -o $@ master.s

clean:
	rm -f *.a *.out test.log
