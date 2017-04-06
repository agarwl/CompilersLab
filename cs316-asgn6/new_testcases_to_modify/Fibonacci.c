/**************************************************************************
			Interpreter + Compiler test file

				Fibinacci.c
				-----------
			     Fibonacci series

This test file finds the fibonacci series to a particular number enetred in variable 'size' using functions
**************************************************************************/

int size;
int fibonacci (int prev_number, int number);
int i;
int prev_number;
int number;

void main()
{
    prev_number=0;
    number=1;
    size = 20;
    i = 1;
    fibonacci (prev_number,number);
}

int fibonacci (int prev_number, int number)
{
    int next_num;

    if (i==size)
      return(0);
    else
    {
      next_num=prev_number+number;
      prev_number=number;
      number=next_num;
      i++; // increment counter
      fibonacci (prev_number,number); //recursion
    }
    return(0);
}
