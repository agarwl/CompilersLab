void main();

main()
{
	float a, b, c, d;
	a = 2.3; b = 3.4;
	d = a+b < a-b ? a*b : a - b; // allowed
	//a+b < a-b ? a*b : a - b; // not allowed
}
