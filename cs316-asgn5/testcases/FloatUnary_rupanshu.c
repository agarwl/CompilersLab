// Relational operators are not associative.

main()
{
	float a = 3.23, j = 5.677;

	a = a * -j / a + -j;

	if (a >= -j < -a)
		a = a + 1;
	else
		j = -a-1;
}
