void main();

main()
{
	float f , g;
	f = -5.5; g = 6.6;	
	while((f > 2*g? f: g+f) > g)
	{
		f = ((f<1)? 1.0 : 2);
	}
}
