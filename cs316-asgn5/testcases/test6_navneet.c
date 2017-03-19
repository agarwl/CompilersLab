void main();
// Boolean condition cannot be an arithmetic expression.

main()
{
	int a, b, c;

	if (a + b * c / a / b)
		a = a+1;
}
