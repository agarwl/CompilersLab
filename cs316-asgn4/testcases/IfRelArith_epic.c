void main();

main()
{
	int a, b, c, d, e, f;
	a = 2; b = 4; c = 10; d = 3; e = 8; f = 7;
	//if (a >= b + c && d < e - a != f) // not allowed because of d < (e-a) != f ; d < (e-a) is boolean but f is not
	//	a = a + d * e / f;
	//else
	//	b = 1;

	if (a >= b + c && d < e - a || f != d) // allowed
		a = a + d * e / f;
	else
		b = 1;

}
