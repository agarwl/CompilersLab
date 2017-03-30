void main();

main()
{
	int a;
	a = 3;
	fn(a, 4);
	return;
}

fn(int a, int b)	// No function prototype for function 'fn'
{
	a = a + b;
	return;
}
