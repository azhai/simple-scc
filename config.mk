# scc version
VERSION = 0.1

## Customize below to fit your system
DRIVER = posix

# Can be c89 or c99
STD = c99

# paths
PREFIX    = $(PWD)/$(PROJECTDIR)/rootdir
MANPREFIX = $(PREFIX)/share/man

# scc expects to be built by a C99 compiler
# if your system is not at least POSIX 2004 compatible, adjust CC
# CC = c99
# AR = ar
AS = as

### Systems
# Plan9
#SYSCFLAGS = -D_SUSV2_SOURCE
###
# NetBSD
#CC1_CFLAGS = -D_ISOC99_SOURCE -D_ANSI_SOURCE
#CC2_CFLAGS = -D_ISOC99_SOURCE -D_ANSI_SOURCE
#AS_CFLAGS = -D_ISOC99_SOURCE -D_ANSI_SOURCE
###
# OpenBSD
#CC1_CFLAGS = -D_ISOC99_SOURCE -D_ANSI_SOURCE
#CC2_CFLAGS = -D_ISOC99_SOURCE -D_ANSI_SOURCE
#AS_CFLAGS = -D_ISOC99_SOURCE -D_ANSI_SOURCE
###
# FreeBSD
#CC1_CFLAGS = -D_ISOC99_SOURCE -D_ANSI_SOURCE
#CC2_CFLAGS = -D_ISOC99_SOURCE -D_ANSI_SOURCE
#AS_CFLAGS = -D_ISOC99_SOURCE -D_ANSI_SOURCE
###
