// Boolean condition cannot be an arithmetic expression.

main()
{
	int a = 3, b = 5, c = 10;

	if (a + b * c / a / b)
		a = a+1;
}
