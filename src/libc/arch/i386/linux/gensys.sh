#!/bin/sh

awk '/^[0-9]*\t/ {
	syscall=$2
	fname=$2".s"
	noper=$3

	printf "\t.file\t"
	       "\"fname\"\n"
	       "\t.globl\t%s\n"
	       "%s:\n"
	       "\tpushl\t%%ebp\n"
	       "\tpushl\t%%ebx\n"
	       "\tpushl\t%%esi\n\n",
	       "\tpushl\t%%edi\n"
	       "\tmovl\t%%esp,%%ebp\n"
	       syscall, syscall > fname

	if (noper > 0)
		printf "\tmovl\t20(%%ebp),%%ebx\n" >> fname
	if (noper > 1)
		printf "\tmovl\t24(%%ebp),%%ecx\n" >> fname
	if (noper > 2)
		printf "\tmovl\t28(%%ebp),%%edx\n" >> fname
	if (noper > 3)
		printf "\tmovl\t28(%%ebp),%%esi\n" >> fname
	if (noper > 4)
		printf "\tmovl\t28(%%ebp),%%edi\n" >> fname

	printf "\tmovl\t$%d,%%eax\n"
	       "\tint\t$0x80\n"
	       "\tpopl\t%%edi\n"
	       "\tpopl\t%%esi\n"
	       "\tpopl\t%%ebx\n"
	       "\tpopl\t%%ebp\n"
	       "\tjmp\t_cerrno\n", $1 >> fname

} ' syscall.lst
