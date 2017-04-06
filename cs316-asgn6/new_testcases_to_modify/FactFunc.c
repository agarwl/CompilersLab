/****************************************************************************
			Interpreter + Compiler test file

				FactFunc.c
				----------
			Calculating factorial using functions

This test case calculates factorial of a number with/without recursion.
***************************************************************************/

void nonrecurfn(int);
void recurfn(int);
int recurfactorial(int);
int nonrecurfactorial(int);
 
int main()
{
  int number;
  int fact = 1;
  number = 5; 

  nonrecurfn(number); 
  recurfn(number);
}

// Recursion

void recurfn(int number)
{
  int fact = recurfactorial(number);
}
 
int recurfactorial(int n)
{
  if (n == 0)
    return 1;
  else
    return(n * recurfactorial(n-1));
}

// No Rescursion

int nonrecurfactorial(int n)
{
  int c;
  int result = 1;
 
  for (c = 1; c <= n; c++)
    result = result * c;
 
  return result;
}

void nonrecurfn(int n)
{
  if (n < 0)
    return ;
  else
  {
    int f = nonrecurfactorial(n);
  }
 
  return ;
}
