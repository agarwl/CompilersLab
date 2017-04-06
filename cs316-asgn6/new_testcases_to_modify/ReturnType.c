fn2(int a, int b)
{
   b = 2;
   return b;
}
main()
{
   int a;
   a = 3;
   a = fn2(a, a);
}
