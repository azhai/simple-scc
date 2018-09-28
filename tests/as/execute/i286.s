/ Look in http://www.mlsite.net/8086/
/ Look in http://www.srmuniv.ac.in/sites/default/files/2017/15CS205J-unitII.pdf
/ Look in http://www.c-jump.com/CIS77/reference/Instructions_by_Opcode.html
/ Look in http://www.c-jump.com/CIS77/CPU/x86/lecture.html#X77_0020_encoding_overview
/ Look in http://www.c-jump.com/CIS77/reference/Instructions_by_Opcode.html
/ Look in https://www.felixcloutier.com/x86/index.html

	ADDB	AL,AL			/ 00 C0
	ADDB	CL,AL			/ 00 C8
	ADDB	DL,AL			/ 00 D0
	ADDB	BL,AL			/ 00 D8
	ADDB	AH,AL			/ 00 E0
	ADDB	CH,AL			/ 00 E8
	ADDB	DH,AL			/ 00 F0
	ADDB	BH,AL			/ 00 F8

	ADDW	AX,AX			/ 01 C0
	ADDW	CX,AX			/ 01 C8
	ADDW	DX,AX			/ 01 D0
	ADDW	BX,AX			/ 01 D8
	ADDW	SP,AX			/ 01 E0
	ADDW	BP,AX			/ 01 E8
	ADDW	SI,AX			/ 01 F0
	ADDW	DI,AX			/ 01 F8

	ADDB	AL,CL			/ 00 C1
	ADDB	CL,CL			/ 00 C9
	ADDB	DL,CL			/ 00 D1
	ADDB	BL,CL			/ 00 D9
	ADDB	AH,CL			/ 00 E1
	ADDB	CH,CL			/ 00 E9
	ADDB	DH,CL			/ 00 F1
	ADDB	BH,CL			/ 00 F9

	ADDW	AX,CX			/ 01 C1
	ADDW	CX,CX			/ 01 C9
	ADDW	DX,CX			/ 01 D1
	ADDW	BX,CX			/ 01 D9
	ADDW	SP,CX			/ 01 E1
	ADDW	BP,CX			/ 01 E9
	ADDW	SI,CX			/ 01 F1
	ADDW	DI,CX			/ 01 F9

	ADDB	AL,DL			/ 00 C2
	ADDB	CL,DL			/ 00 CA
	ADDB	DL,DL			/ 00 D2
	ADDB	BL,DL			/ 00 DA
	ADDB	AH,DL			/ 00 E2
	ADDB	CH,DL			/ 00 EA
	ADDB	DH,DL			/ 00 F2
	ADDB	BH,DL			/ 00 FA

	ADDW	AX,DX			/ 01 C2
	ADDW	CX,DX			/ 01 CA
	ADDW	DX,DX			/ 01 D2
	ADDW	BX,DX			/ 01 DA
	ADDW	SP,DX			/ 01 E2
	ADDW	BP,DX			/ 01 EA
	ADDW	SI,DX			/ 01 F2
	ADDW	DI,DX			/ 01 FA

	ADDB	AL,BL			/ 00 C3
	ADDB	CL,BL			/ 00 CB
	ADDB	DL,BL			/ 00 D3
	ADDB	BL,BL			/ 00 DB
	ADDB	AH,BL			/ 00 E3
	ADDB	CH,BL			/ 00 EB
	ADDB	DH,BL			/ 00 F3
	ADDB	BH,BL			/ 00 FB

	ADDW	AX,BX			/ 01 C3
	ADDW	CX,BX			/ 01 CB
	ADDW	DX,BX			/ 01 D3
	ADDW	BX,BX			/ 01 DB
	ADDW	SP,BX			/ 01 E3
	ADDW	BP,BX			/ 01 EB
	ADDW	SI,BX			/ 01 F3
	ADDW	DI,BX			/ 01 FB

	ADDB	AL,AH			/ 00 C4
	ADDB	CL,AH			/ 00 CC
	ADDB	DL,AH			/ 00 D4
	ADDB	BL,AH			/ 00 DC
	ADDB	AH,AH			/ 00 E4
	ADDB	CH,AH			/ 00 EC
	ADDB	DH,AH			/ 00 F4
	ADDB	BH,AH			/ 00 FC

	ADDW	AX,SP			/ 01 C4
	ADDW	CX,SP			/ 01 CC
	ADDW	DX,SP			/ 01 D4
	ADDW	BX,SP			/ 01 DC
	ADDW	SP,SP			/ 01 E4
	ADDW	BP,SP			/ 01 EC
	ADDW	SI,SP			/ 01 F4
	ADDW	DI,SP			/ 01 FC

	ADDB	AL,CH			/ 00 C5
	ADDB	CL,CH			/ 00 CD
	ADDB	DL,CH			/ 00 D5
	ADDB	BL,CH			/ 00 DD
	ADDB	AH,CH			/ 00 E5
	ADDB	CH,CH			/ 00 ED
	ADDB	DH,CH			/ 00 F5
	ADDB	BH,CH			/ 00 FD

	ADDW	AX,BP			/ 01 C5
	ADDW	CX,BP			/ 01 CD
	ADDW	DX,BP			/ 01 D5
	ADDW	BX,BP			/ 01 DD
	ADDW	SP,BP			/ 01 E5
	ADDW	BP,BP			/ 01 ED
	ADDW	SI,BP			/ 01 F5
	ADDW	DI,BP			/ 01 FD

	ADDB	AL,DH			/ 00 C6
	ADDB	CL,DH			/ 00 CE
	ADDB	DL,DH			/ 00 D6
	ADDB	BL,DH			/ 00 DE
	ADDB	AH,DH			/ 00 E6
	ADDB	CH,DH			/ 00 EE
	ADDB	DH,DH			/ 00 F6
	ADDB	BH,DH			/ 00 FE

	ADDW	AX,SI			/ 01 C6
	ADDW	CX,SI			/ 01 CE
	ADDW	DX,SI			/ 01 D6
	ADDW	BX,SI			/ 01 DE
	ADDW	SP,SI			/ 01 E6
	ADDW	BP,SI			/ 01 EE
	ADDW	SI,SI			/ 01 F6
	ADDW	DI,SI			/ 01 FE

	ADDB	AL,BH			/ 00 C7
	ADDB	CL,BH			/ 00 CF
	ADDB	DL,BH			/ 00 D7
	ADDB	BL,BH			/ 00 DF
	ADDB	AH,BH			/ 00 E7
	ADDB	CH,BH			/ 00 EF
	ADDB	DH,BH			/ 00 F7
	ADDB	BH,BH			/ 00 FF

	ADDW	AX,DI			/ 01 C7
	ADDW	CX,DI			/ 01 CF
	ADDW	DX,DI			/ 01 D7
	ADDW	BX,DI			/ 01 DF
	ADDW	SP,DI			/ 01 E7
	ADDW	BP,DI			/ 01 EF
	ADDW	SI,DI			/ 01 F7
	ADDW	DI,DI			/ 01 FF