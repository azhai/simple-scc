static char sccsid[] = "@(#) ./as/target/x86/ins.c";

#include <stdlib.h>

#include "../../../inc/scc.h"
#include "../../as.h"
#include "proc.h"

static int
getclass(Node *np)
{
	if (np->addr != AREG)
		return 0;

	switch (np->sym->value) {
	case AREG_AL:
	case AREG_AH:
	case AREG_BL:
	case AREG_BH:
	case AREG_CL:
	case AREG_CH:
	case AREG_DL:
	case AREG_DH:
		return R8CLASS;

	case AREG_CS:
	case AREG_DS:
	case AREG_SS:
	case AREG_ES:
	case AREG_FS:
	case AREG_GS:

	case AREG_EFLAGS:
	case AREG_CF:
	case AREG_PF:
	case AREG_AF:
	case AREG_ZF:
	case AREG_SF:
	case AREG_TF:
	case AREG_IF:
	case AREG_DF:
	case AREG_OF:
	case AREG_IOPL:
	case AREG_NT:
	case AREG_RF:
	case AREG_VM:
	case AREG_AC:
	case AREG_VIF:
	case AREG_VIP:
	case AREG_ID:

	case AREG_AX:
	case AREG_EAX:
	case AREG_RAX:

	case AREG_BX:
	case AREG_EBX:
	case AREG_RBX:

	case AREG_CX:
	case AREG_ECX:
	case AREG_RCX:

	case AREG_DX:
	case AREG_EDX:
	case AREG_RDX:

	case AREG_SI:
	case AREG_SIL:
	case AREG_ESI:
	case AREG_RSI:
	case AREG_DI:
	case AREG_DIL:
	case AREG_EDI:
	case AREG_RDI:

	case AREG_SP:
	case AREG_SPL:
	case AREG_ESP:
	case AREG_RSP:

	case AREG_BP:
	case AREG_BPL:
	case AREG_EBP:
	case AREG_RBP:

	case AREG_R0:
	case AREG_MM0:
	case AREG_R1:
	case AREG_MM1:
	case AREG_R2:
	case AREG_MM2:
	case AREG_R3:
	case AREG_MM3:
	case AREG_R4:
	case AREG_MM4:
	case AREG_R5:
	case AREG_MM5:
	case AREG_R6:
	case AREG_MM6:
	case AREG_R7:
	case AREG_MM7:

	case AREG_R8:
	case AREG_R8L:
	case AREG_R8W:
	case AREG_R9:
	case AREG_R9L:
	case AREG_R9W:
	case AREG_R10:
	case AREG_R10L:
	case AREG_R10W:
	case AREG_R11:
	case AREG_R11L:
	case AREG_R11W:
	case AREG_R12:
	case AREG_R12L:
	case AREG_R12W:
	case AREG_R13:
	case AREG_R13L:
	case AREG_R13W:
	case AREG_R14:
	case AREG_R14L:
	case AREG_R14W:
	case AREG_R15:
	case AREG_R15L:
	case AREG_R15W:

	case AREG_XMM0:
	case AREG_XMM1:
	case AREG_XMM2:
	case AREG_XMM3:
	case AREG_XMM4:
	case AREG_XMM5:
	case AREG_XMM6:
	case AREG_XMM7:
	case AREG_XMM8:
	case AREG_XMM9:
	case AREG_XMM10:
	case AREG_XMM11:
	case AREG_XMM12:
	case AREG_XMM13:
	case AREG_XMM14:
	case AREG_XMM15:

	case AREG_YMM0:
	case AREG_YMM1:
	case AREG_YMM2:
	case AREG_YMM3:
	case AREG_YMM4:
	case AREG_YMM5:
	case AREG_YMM6:
	case AREG_YMM7:
	case AREG_YMM8:
	case AREG_YMM9:
	case AREG_YMM10:
	case AREG_YMM11:
	case AREG_YMM12:
	case AREG_YMM13:
	case AREG_YMM14:
	case AREG_YMM15:

	case AREG_MXCSR:
		return 0;
	default:
		abort();
	}
}

int
match(Op *op, Node **args)
{
	unsigned char *p;
	int arg, class;
	Node *np;

	if (!op->args)
		return args == NULL;

	for (p = op->args; arg = *p; ++p) {
		if (arg & AREP)
			--p;
		if ((np = *args++) == NULL)
			return (arg & (AREP|AOPT)) != 0;

		arg &= ~(AREP|AOPT);
		switch (arg) {
		case AREG_R8CLASS:
			class = R8CLASS;
		check_class:
			if ((getclass(np) & class) == 0)
				return 0;
			break;
		case AIMM8:
		case AIMM16:
		case AIMM32:
		case AIMM64:
			if (np->addr != AIMM)
				return 0;
			if (toobig(np, arg))
				error("overflow in immediate operand");
			break;
		case ASYM:
			if (np->addr != AIMM || np->op != IDEN)
				return 0;
			break;
		case ADIRECT:
		case ASTR:
			if (np->addr != arg)
				return 0;
			break;
		default:
			abort();
		}
	}

	return *args == NULL;
}

Node *
moperand(void)
{
}
