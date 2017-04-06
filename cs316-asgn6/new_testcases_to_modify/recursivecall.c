int r(int a)
{
if(a==1)
return 1;
else
{
int i;
i = r(a-1);
return i + 1;
}

}

main()
{
int x,y,z;
x = 5;

y = r(x);
z = r(3); 

}
