// Empty statement in if statement is allowed.
void main();
main()
{
	int f;
	if (f < 0)f=1;
	else f = 0;
	while (f < 0)f=1;
	while (f > 0)f=1;
	while (f <= 0)f=1;
	while (f >= 0)f=1;
	while (f == 0)f=1;
	while (f != 0)f=1;
	while ((f >= 0)&&(f >= 0))f=1;
	while ((f >= 0)||(f >= 0))f=1;
	while (!(f >= 0))f=1;
	do f = 1;while (!(f >= 0));
	f = f==0?f:0;
	// fkjghrigbiu
	f = f + 1;
	if(f==0)if(f<0)f=0;else f = 1;
	if(f<0)f=0;
	else {f = 1;f = 3;}
	if(f+10<0)f=0;
	if(f+10<f*100)f=0;
	if(f+10<=f*100)f=0;

}
