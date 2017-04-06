/****************************************************************************
			Interpreter + Compiler test file

				TowerOfHanoi.c
				--------------
				Tower of hanoi

This test file implements tower of hanoi.
***************************************************************************/

int from_tower;
int to_tower;
void TOH(int n, int x,int y,int z);
void main()
{
 int n;
 n = 5;
 TOH(n-1,100,200,300);
}
void TOH(int n,int x,int  y,int z)
{
 if(n>0)
 {
  TOH(n-1,x,z,y);
  from_tower = x;
  to_tower = y;
  TOH(n-1,z,y,x);
 }
}
