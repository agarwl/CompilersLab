// Boolean condition cannot be an arithmetic expression.
void main();

main()
{
	int a, b, c;
	a = 3; b = 5; c = 10;
	if (a + b * c)
		a = a+1;
}
