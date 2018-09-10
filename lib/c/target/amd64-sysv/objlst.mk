ARCHOBJ = setjmp.o longjmp.o

setjmp.o: ../amd64-sysv/setjmp.s
	$(AS) $(ASFLAGS) -o $@ ../amd64-sysv/setjmp.s

longjmp.o: ../amd64-sysv/longjmp.s
	$(AS) $(ASFLAGS) -o $@ ../amd64-sysv/longjmp.s
