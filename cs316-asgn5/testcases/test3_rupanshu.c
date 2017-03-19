void main();

main()
{
	float f , g;
	f = -5.5; g = 6.6;

	if (!(f < g))
		f = 3.0;
	f = ((f>=g || g == f || g > f || g < f || g != f) && (!(g > f) || !(g == f)))? f: g;

	int f , g;
	f = -5; g = 6;

	if (!(f < g))
		f = 3;

}
