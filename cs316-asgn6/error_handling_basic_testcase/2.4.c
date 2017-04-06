int f(int a, int b, float c);
void main(int a);
int g();
float b;

main(int a)
{
	// int f;
	int b;
	b=2;
	a = f(1+2, 2*b , g());
	return;
}


f(int a, int b, float c)
{
	return 1;
}
g()
{
	return 1;
}