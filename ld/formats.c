static char sccsid[] = "@(#) ./ld/probe.c";

#include <stdio.h>

#include "ld.h"

/* TODO: Autogenerate this file */
struct objfile coff32;

struct objfile *formats[] = {
	&coff32,
	NULL,
};
