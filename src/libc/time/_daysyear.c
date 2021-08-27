int _daysmon[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

int
_daysyear(int year)
{
	if (year%4 != 0)
		return 365;
	if (year%100 == 0 && year%400 != 0)
		return 365;
	return 366;
}
