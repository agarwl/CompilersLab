int f(int a, int b);
int g();
void main(int a);


main(int a)
{
	int f,g;
	f(1,1);
	g();
	return;
}


f(int a, int b)
{
	return 1;
}

g()
{
	return 2;
}