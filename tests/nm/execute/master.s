	.globl	text1,averylongtext
	.text
	.equ	text2,4
text1:	.byte	0
averylongtext:
	.byte	0
text3:	.byte	0

	.globl	data1,averylongdata
	.data
	.equ	data2,5
data1:	.byte	3
averylongdata:
	.byte	0
data3:	.byte	0

	.globl	bss1,averylongbss
	.bss
	.equ	bss2,5
bss1:	.byte	0
averylongbss:
	.byte	0
bss3:	.byte	0
