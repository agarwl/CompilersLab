void main(); main ()
{
  int a;
  int b;
  a = (2 + 2) - 2 ;
  // b = (2 * 2);
}

./sclp testcases/temp.c -d -ast | head -n 14
Program:
   Procedure: main, Return Type: void
      Sequence Ast 
         Asgn:
            LHS (Name : a)
            RHS (
            Arith: MINUS
               LHS (
               Arith: PLUS
                  LHS (Num : 2)
                  RHS (Num : 2))
               RHS (Num : 2))

rishabh@rishabh-Vostro-2520:~/sem6/302 - IPL/labs/cs316-asgn2$
./sclp16 testcases/temp.c -d -ast | head -n 14
Program:
   Procedure: main, Return Type: void
      Sequence Ast 

         Asgn:
            LHS (Name : a)
            RHS (
            Arith: MINUS
               LHS (
            Arith: PLUS
               LHS (Num : 2)
               RHS (Num : 2))
               RHS (Num : 2))
