PROFILE_CFLAGS  = -fprofile-arcs -ftest-coverage
PROFILE_LDLIBS  = -lgcov
include $(BUILDDIR)/tool/gnu.mk

coverage:
	mkdir -p coverage
	lcov -c -d . -o coverage/cov.info
	genhtml -o coverage coverage/cov.info
