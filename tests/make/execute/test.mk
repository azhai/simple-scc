
.IGNORE: ignored-error
.SILENT: silent-echo-hello

follow: error hello

hello:
	@echo Hello World!

silent-echo-hello:
	echo Hello World!

echo-hello:
	echo Hello World!

error:
	no-valid-program

ignored-error:
	no-valid-program

print-make:
	@echo $(MAKE)

print-cc:
	@echo $(CC)

print-env-cc:
	@echo $$CC

print-makeflags:
	@echo $(MAKEFLAGS)

print-makeflags-always:
	+@echo $(MAKEFLAGS)

print-hidden-hello:
	+@echo Hello World!

file1:
	echo > file1

file2:

file.o:

test.mk:
	touch $@
