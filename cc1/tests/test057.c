
/*
name: TEST057
description: test of initialization of incomplete arrays
error:
output:
V1	I	#3
V2	V1	#2
G3	V2	"arr1
(
	#I2
	#I7
	#I5
	#I5
	#I1
	#I2
)
G4	V2	"arr2
(
	#I2
	#I7
	#I5
	#I5
	#I1
	#I2
)
F5	I
G6	F5	"main
{
\
	r	G3	'P	#P6	+P	#P4	+P	@I	G4	'P	#P6	+P	#P6	+P	@I	!I
}
*/

int arr1[][3] = {
                 { 2, 7, 5, },
                 { 5, 1, 2, },
};

int arr2[2][3] = {
	2, 7, 5,
	5, 1, 2
};

int
main(void)
{
	return !(arr1[1][2] == arr2[1][3]);
}
