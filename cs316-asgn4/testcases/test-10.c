void main(); 

main()

{
	int a;
	float b;

	if(!(a < 1 && b > 1.1))
		if (b == 1.0)
			b = 1.1;
		else if(b == 1.1)
			b = 1.0;
		else
		{}
}