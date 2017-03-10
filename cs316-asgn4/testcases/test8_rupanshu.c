void main();

main()
{
	float f , g;
	f = -5.5; g = 6.6;

	f = ((f>=g || g == f || g > f || g < f || g != f) && (!(g > f) || !(g == f)))? f: g;
}
