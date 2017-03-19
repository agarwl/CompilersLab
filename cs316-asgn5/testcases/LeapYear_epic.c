void main();

main()
{
  int year;
  int cntr;	// cntr = 1 - leap year
		// cntr = 0 - not leap year

  year = 1990; 
 
  if ( (year/400)*400 == year)		// % is replaced by / and *
	cntr = 1;
  else if ( (year/100)*100 == year)
	cntr = 0;
  else if ( (year/4)*4 == year )
	cntr = 1;
  else
	cntr = 0;

}
