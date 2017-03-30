// Empty statement in if statement is allowed.
void main();
main()
{
	int a;
	int b;
	int f;
	while ((f < 0)&&(f>0)){
		f = 0;
		f = 1;
	}
	f = (f<0)?a:b;
	if ((f < 0)&&(f>0)){}
	{
		f = 0;
		f = 1;
	}

}
