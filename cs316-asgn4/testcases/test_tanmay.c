void main();

main()
{
	int x,y;
	int a, j, b, c;
	float a1, j1;

	if (x != y)
		x = -y;


	if (a == b) {
		if (b != c) {
			j = a;
			if(a==b)
				a=2;
		}
		else {
			a = 1;
		}
	}

	a1 = 1.0;
	{
		j1 = a1 + 1.0;
		j1 = -a1;
	}
	a1 = 3.23;
	j1 = 5.677;

	if ( !(a < j) ) {
		a1 = a1 + 1.0;
	}
	else
	{
		if (!(a > j))
		j1 = -a1-1.0;
	}

	while(a == j) {
		a = j;
		j = a; 
		{
			a = j;
			j = a;
			{
				a = j;
			}
		}
	}
	a = j;
	j = a;
	
	a = 1;

	if (a > 2) {
		j = 1;
		j = 3;
	}
	else {
		j = 2;
	}

	a = (a>b)?a:b;

	while (a < 10)
	{
		a = 2 + j;
	}

	do {
		a = 1;
		b = 2;
	}
	while (j <= -1);


	a1 = 1.0;
	a1 = a1 + j1;

}
