// Relational operators are not associative.
void main();

main()
{
	float a, j;
	a = 3.23; j = 5.677;
	
	a = a * -j / a + -j;

	if (a >= -j)
		a = a + 1.0;
	else
		j = -a-1.0;
}
