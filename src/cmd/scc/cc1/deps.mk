#deps
./builtin.o: $(INCDIR)/scc/scc/scc.h
./builtin.o: .cc1.h
./code.o: $(INCDIR)/scc/scc/scc.h
./code.o: .cc1.h
./cpp.o: $(INCDIR)/scc/scc/cstd.h
./cpp.o: $(INCDIR)/scc/scc/scc.h
./cpp.o: .cc1.h
./decl.o: $(INCDIR)/scc/scc/cstd.h
./decl.o: $(INCDIR)/scc/scc/scc.h
./decl.o: .cc1.h
./error.o: $(INCDIR)/scc/scc/scc.h
./error.o: .cc1.h
./expr.o: $(INCDIR)/scc/scc/cstd.h
./expr.o: $(INCDIR)/scc/scc/scc.h
./expr.o: .cc1.h
./fold.o: $(INCDIR)/scc/scc/scc.h
./fold.o: .cc1.h
./init.o: $(INCDIR)/scc/scc/cstd.h
./init.o: $(INCDIR)/scc/scc/scc.h
./init.o: .cc1.h
./lex.o: $(INCDIR)/scc/scc/cstd.h
./lex.o: $(INCDIR)/scc/scc/scc.h
./lex.o: .cc1.h
./main.o: $(INCDIR)/scc/scc/arg.h
./main.o: $(INCDIR)/scc/scc/scc.h
./main.o: .cc1.h
./stmt.o: $(INCDIR)/scc/scc/cstd.h
./stmt.o: $(INCDIR)/scc/scc/scc.h
./stmt.o: .cc1.h
./symbol.o: $(INCDIR)/scc/scc/cstd.h
./symbol.o: $(INCDIR)/scc/scc/scc.h
./symbol.o: .cc1.h
./target/amd64-sysv/arch.o: $(INCDIR)/scc/scc/scc.h
./target/amd64-sysv/arch.o: ./target/amd64-sysv../../cc1.h
./target/arm64-sysv/arch.o: $(INCDIR)/scc/scc/scc.h
./target/arm64-sysv/arch.o: ./target/arm64-sysv../../cc1.h
./target/i386-sysv/arch.o: $(INCDIR)/scc/scc/scc.h
./target/i386-sysv/arch.o: ./target/i386-sysv../../cc1.h
./target/z80-scc/arch.o: $(INCDIR)/scc/scc/scc.h
./target/z80-scc/arch.o: ./target/z80-scc../../cc1.h
./types.o: $(INCDIR)/scc/scc/cstd.h
./types.o: $(INCDIR)/scc/scc/scc.h
./types.o: .cc1.h
