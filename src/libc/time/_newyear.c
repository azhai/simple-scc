/*
 * Happy New Year!!!!
 */
int
_newyear(int year)
{
	int day;

	year += 1900 - 1;
	day = 1 + year + year/4;
	day -= year/100;
	day += year/400;

	return day % 7;
}
