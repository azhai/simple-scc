PROFILE_CFLAGS  = -fprofile-arcs -ftest-coverage -fprofile-abs-path
PROFILE_LDLIBS  = -lgcov
include $(BUILDDIR)/tool/gnu.mk
