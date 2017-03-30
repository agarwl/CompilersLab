void main();

main()
{
   int n;
   int cntr;	// cntr = 1, even
		// cntr = 0, odd
 
   n = 3;

// 1st way of finding whether a number is odd or even.
 
   //if ( n & 1 == 0 )
     // cntr = 1;
   //else 
     // cntr = 0;
 
// 2nd way of finding whether a number is odd or even.

   if ( (n/2)*2 == n )	// % is replaced with / and *
      cntr = 1;
   else
      cntr = 0;


}
