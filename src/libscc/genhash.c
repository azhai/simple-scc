unsigned
genhash(char *name)
{
	int c;
	unsigned h = 5381;

	while (c = *name++)
		h = h*33 ^ c;

	return h;
}
