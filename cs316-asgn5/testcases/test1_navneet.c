// Relational operators are not associative.
void main();

main()
{
	// float a, j;
	// int b,c;
	// a = 1.0;
	// a = 1.0;
	// a = j;
	// a = a + 1.0;
	// a = a / j;
	// a = (0<0)?a:j;
	// a = a + a;

	// if((b<c)){
	// 	b = b+c;
	// }
	// {
	// 	j = a + 1.0;
	// 	j = -a;
	// }
	// a = 3.23;
	// j = 5.677;
	
	// a = a * -j / a + -j;

	// if (a >= -j)
	// 	a = a + 1.0;
	// else
	// 	j = -a-1.0;

	// int a, j, b;
	// a = 1;

	// if (a > 2) {
	// 	j = 1;
	// }
	// else {
	// 	j = 2;
	// }

	// a = (a>b)?a:b;

	// if( a < 10.0)
	// 	a = 2.0 + j;
	//  int x,y;
	int a, j, b, c;
	float a1, j1;
	int x, y;

	if (x != y)
		x = y;


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
	
	a1 = a1 * -j1 / a1 + -j1;

	// if ( !(a < j) ) {
	// 	// a = a + 1.0;
	// }

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

	// else
	// {
	// 	// if (!(a > j))
	// 	j = -a-1.0;
	// }

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

	// float a,b;

	// a = 1.0;
	// a = a + b;
  // lcm = (x*y)/gcd;
	// do {
	// 	a = 1;
	// 	b = 2;
	// }
	// while (j <= -1);

	// float a,b;

	// a = 1.0;
	// a = a + b;

}
