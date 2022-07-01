.POSIX:

ROOT=../../..

OUT = z80.out
AS = z80-unknown-coff-as

all:
	@if $(AS) 2>/dev/null;\
	then\
		$(MAKE) tests;\
	fi

tests: $(OUT)
	@PATH=$(ROOT)/bin:$$PATH:. chktest.sh

z80.out: master.s
	$(AS) -o $@ master.s

clean:
	rm -f *.a *.out test.log
