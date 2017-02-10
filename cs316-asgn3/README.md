

This level **extends level 2 by supporting intra-procedural control flow statements and evaluation of conditions** in the following ways:

+ Conditional expression e1?e2:e3, while, do-while, if, and if-else statements are allowed. Other statements such as for, switch, and goto are not supported.  
+ The boolean conditions controlling the control flow consist of the six relational operators (<, <=, >, >=, ==, and !=) and three logical operators (&&, ||, and !). This requires supporting relational expressions which can only take arithmetic expressions as operands to (internally) compute values true and false. Unlike C, arithmetic expressions cannot be used as boolean expressions (or operands) of boolean expressions; only relational expression can be used as boolean expressions or their operands. The above design decision has been motivated by a cleaner type system that avoids mixing values without introducing a bool type.  
+ The right hand sides of assignment statements continue to remain arithmetic expressions only. Further, values of int and float type are not allowed to be mixed.  
+ C++ style comments beginning with // and stretching upto the end of a line are supported.  
+ Type declaration statement now may contain a comma separated list.  

More information can be found [here](https://www.cse.iitb.ac.in/~uday/sclp-web/)!
