
void main();

main(){
  int a, b, x, y, t, gcd, lcm;
  x = 14;
  y = 36; 
 
  a = x;
  b = y;
 
  do {
    t = b;
    b = (a / b) * b;
    a = t;
  } while (b != 0)
 
  gcd = a;
  lcm = (x*y)/gcd;
}
