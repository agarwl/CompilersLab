// 2) A procedure should have same type of return value (int / float) except that a void return can exist along with either int or
   // float return (i.e. a procedure may be declared to be an int/float but may not return a value)


int f();
float g();
int main();

main()
{
	return;
	return 0;
}

f()
{
	return 1.0;
}

g()
{
	return 1;
}