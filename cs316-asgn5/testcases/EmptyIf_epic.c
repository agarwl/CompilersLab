void main();

main()
{
	int i;
	int a;

	i = -2;
	a = 2;
	if (a < 2) {
		a = 3;
		if (a == 2) i = 2;
	}
	else {
		i = 3;
	}

	while (i<10) i=i+1;

	// a = (i>=1)?2:i;

}