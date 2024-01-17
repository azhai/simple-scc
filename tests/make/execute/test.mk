
.IGNORE: ignored-error

error:
	no-valid-program

ignored-error:
	no-valid-program

print-cc:
	@echo $(CC)

print-makeflags:
	@echo $(MAKEFLAGS)
