cpp.c: $(PROJECTDIR)/inc/$(STD)/cstd.h
decl.c: $(PROJECTDIR)/inc/$(STD)/cstd.h
expr.c: $(PROJECTDIR)/inc/$(STD)/cstd.h
init.c: $(PROJECTDIR)/inc/$(STD)/cstd.h
lex.c: $(PROJECTDIR)/inc/$(STD)/cstd.h
stmt.c: $(PROJECTDIR)/inc/$(STD)/cstd.h
symbol.c: $(PROJECTDIR)/inc/$(STD)/cstd.h
types.c: $(PROJECTDIR)/inc/$(STD)/cstd.h

#deps
builtin.o: ../inc/scc.h
builtin.o: cc1.h
code.o: ../inc/scc.h
code.o: cc1.h
cpp.o: ../inc/scc.h
cpp.o: cc1.h
decl.o: ../inc/scc.h
decl.o: cc1.h
error.o: ../inc/scc.h
error.o: cc1.h
expr.o: ../inc/scc.h
expr.o: cc1.h
fold.o: ../inc/scc.h
fold.o: cc1.h
init.o: ../inc/scc.h
init.o: cc1.h
lex.o: ../inc/scc.h
lex.o: cc1.h
main.o: ../inc/arg.h
main.o: ../inc/scc.h
main.o: cc1.h
stmt.o: ../inc/scc.h
stmt.o: cc1.h
symbol.o: ../inc/scc.h
symbol.o: cc1.h
target/amd64-sysv/arch.o: target/amd64-sysv/../../../inc/scc.h
target/amd64-sysv/arch.o: target/amd64-sysv/../../cc1.h
target/arm64-sysv/arch.o: target/arm64-sysv/../../../inc/scc.h
target/arm64-sysv/arch.o: target/arm64-sysv/../../cc1.h
target/i386-sysv/arch.o: target/i386-sysv/../../../inc/scc.h
target/i386-sysv/arch.o: target/i386-sysv/../../cc1.h
target/z80-scc/arch.o: target/z80-scc/../../../inc/scc.h
target/z80-scc/arch.o: target/z80-scc/../../cc1.h
types.o: ../inc/scc.h
types.o: cc1.h
