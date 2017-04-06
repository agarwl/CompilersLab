/**************************************************************************
			Interpreter + Compiler test file

				nCrnPr.c
				--------
			Combination and permutation

This test file calcultes the combination and permutation using two variables n and r.
****************************************************************************/

int factorial(int);
int find_ncr(int, int);
int find_npr(int, int);
 
main()
{
   int n, r;
   int ncr, npr;
 
   n = 5;
   r = 3;
 
   ncr = find_ncr(n, r);
   npr = find_npr(n, r);
 
   return 0;
}
 
int find_ncr(int n, int r)
{
   int result;
 
   result = factorial(n)/(factorial(r)*factorial(n-r));
 
   return result;
}
 
int find_npr(int n, int r)
{
   int result;
 
   result = factorial(n)/factorial(n-r);
 
   return result;
} 
 
int factorial(int n)
{
   int c;
   int result = 1;
 
   for( c = 1 ; c <= n ; c++ )
      result = result*c;
 
   return ( result );
}
