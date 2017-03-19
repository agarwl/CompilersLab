void main();

main()
{
	float f , g;
	f = -5.5; g = 6.6;
	while((f > g? f: g+f) > g)
	{
		f = f*((f<1.0)? 1.0 : 2.0);
	}
}
