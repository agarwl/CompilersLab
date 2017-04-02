
This level **extends Level 3** by supporting function calls (including recursion). In this level, we have multiple functions. Besides, the functions may take arguments and may return results. For simplicity, we assume that function calls do not appear as operands in expression; this requirement is met by first assigning the result of a function call to a source variable and then using the source variable as an operand in the expression at the required place. 

More information can be found [here](https://www.cse.iitb.ac.in/~uday/sclp-web/)

TO FILL:
* `ast-compile.cc`
* `icode.hh`
* `icode.cc`


Note that in the current implementation we have introduced print function. We will now be using print statements in our test cases to check if your program executes correctly. Since we will not check if the generated icode/spim matches reference implementation, you can continue from either Assignment 4 or Assignment 5(Dead code elimination).

////////////////////////

Regarding grammar: The last line of every procedure should be a return statement. 

////////////////////////

program.hh:
	bool variable_in_proc_map_check(string symbol); -> checks for procedure name in existing group of procedures
