					/ Look in http://www.mlsite.net/8086/
					/ Look in http://www.srmuniv.ac.in/sites/default/files/2017/15CS205J-unitII.pdf
/addb reg8,reg8
	ADDB	%AL,%AL			/ 00 C0
	ADDB	%CL,%AL			/ 00 C8
	ADDB	%DL,%AL			/ 00 D0
	ADDB	%BL,%AL			/ 00 D8
	ADDB	%AH,%AL			/ 00 E0
	ADDB	%CH,%AL			/ 00 E8
	ADDB	%DH,%AL			/ 00 F0
	ADDB	%BH,%AL			/ 00 F8

	ADDB	%AL,%CL			/ 00 C1
	ADDB	%CL,%CL			/ 00 C1
	ADDB	%DL,%CL			/ 00 D1
	ADDB	%BL,%CL			/ 00 D9
	ADDB	%AH,%CL			/ 00 E1
	ADDB	%CH,%CL			/ 00 E9
	ADDB	%DH,%CL			/ 00 F1
	ADDB	%BH,%CL			/ 00 F9

	ADDB	%AL,%DL			/ 00 C2
	ADDB	%CL,%DL			/ 00 C2
	ADDB	%DL,%DL			/ 00 D2
	ADDB	%BL,%DL			/ 00 DA
	ADDB	%AH,%DL			/ 00 E2
	ADDB	%CH,%DL			/ 00 EA
	ADDB	%DH,%DL			/ 00 F2
	ADDB	%BH,%DL			/ 00 FA

/addw reg16,reg16
	ADDW	%AX,%AX			/ 01 C0
	ADDW	%CX,%AX			/ 01 C8
	ADDW	%DX,%AX			/ 01 D0
	ADDW	%BX,%AX			/ 01 D8
	ADDW	%SP,%AX			/ 01 E0
	ADDW	%BP,%AX			/ 01 E8
	ADDW	%SI,%AX			/ 01 F0
	ADDW	%DI,%AX			/ 01 F8

--
	ADDB	%AL,(%AX)		/ 02
	ADDW	%AX,(%AX)		/ 03
	ADDB	64,%AL			/ 04 40
	ADDW	32,%AX			/ 05 20
