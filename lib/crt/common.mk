all: $(TARGETS)

$(CRT): crt.o
	cp crt.o $@

clean:
	rm -f *.o
	rm -f $(TARGETS)
