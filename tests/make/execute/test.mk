
.IGNORE: ignored-error

follow: error hello

hello:
	@echo Hello World!

echo-hello:
	echo Hello World!

error:
	no-valid-program

ignored-error:
	no-valid-program

print-cc:
	@echo $(CC)

print-makeflags:
	@echo $(MAKEFLAGS)

print-makeflags-always:
	+@echo $(MAKEFLAGS)

print-hidden-hello:
	+@echo Hello World!

file.o:

test.mk:
	touch $@
