#deps
ar-posix.o: ar.h
ar.o: $(INCDIR)/scc/scc/ar.h
ar.o: $(INCDIR)/scc/scc/arg.h
ar.o: ar.h
nm.o: $(INCDIR)/scc/scc/arg.h
nm.o: $(INCDIR)/scc/scc/mach.h
size.o: $(INCDIR)/scc/scc/arg.h
size.o: $(INCDIR)/scc/scc/mach.h
strip.o: $(INCDIR)/scc/scc/arg.h
strip.o: $(INCDIR)/scc/scc/mach.h
