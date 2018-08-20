
all: libc.a

libc.a: $(OBJ)
	$(AR) $(ARFLAGS) $@ $?
	ranlib $@

clean:
	rm -f *.o *.a
