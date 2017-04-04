%scanner scanner.h
%scanner-token-function d_scanner.lex()
%filenames parser
%parsefun-source parser.cc

%union
{

	pair<Data_Type, list<string>* > * decl;
	list<string> * var_names;
	Symbol_Table * symbol_table;
	Symbol_Table_Entry * symbol_table_entry;
	list<Symbol_Table_Entry* > * symbol_entry_list;
	Procedure * procedure;
	Data_Type data_type;
	list<Ast*> * ast_list;
	// list<Procedure*> * proc_list;

	//ADD CODE HERE
	Sequence_Ast * sequence_ast;
	Call_Ast * call_ast;
	Iteration_Statement_Ast * iteration_ast;
	Conditional_Operator_Ast * condition_ast;
	Relational_Expr_Ast * relational_ast;
	Ast * ast;
	Return_Ast * return_ast;
	Print_Ast * print_ast;
	int integer_value;
	std::string * string_value;
	double double_value;
};

//ADD TOKENS HERE
%token <integer_value> INTEGER_NUMBER
%token <string_value> STRING
%token <string_value> NAME
%token INTEGER
%token ASSIGN VOID RETURN PRINT
%token <double_value> DOUBLE_NUMBER
%token FLOAT
%token DO WHILE IF FOR


%nonassoc THEN
%nonassoc ELSE
%right TERNARY_COND
%left OR
%left AND
%left EQ NE
%left GT GE LT LE
%left '+' '-'
%left '*' '/'
%right UMINUS NOT
%nonassoc '('

%type <symbol_table> optional_variable_declaration_list
%type <symbol_table> variable_declaration_list
// %type <procedure> procedure_declaration
// %type <proc_list> proc_declaration_list
%type <data_type> return_data_type
%type <symbol_table_entry> argument_declaration
%type <symbol_table> argument_declaration_list
%type <symbol_table> optional_argument_list
%type <symbol_entry_list> variable_declaration
%type <decl> declaration
%type <var_names> name_list
%type <sequence_ast> statement_list
%type <ast> assignment_statement
%type <ast> for_statement
%type <ast> variable
%type <ast> constant
%type <ast> operand
%type <ast> arith_expression
%type <ast> expression_term
%type <relational_ast> relational_expression
%type <ast> bool_expression
%type <ast> other_statement
%type <ast> statement
%type <iteration_ast> do_while_statement
%type <iteration_ast> while_matched_statement
%type <sequence_ast> for_matched_statement
%type <call_ast> function_call_statement
%type <ast_list> function_argument_list
%type <ast_list> optional_function_argument_list
%type <return_ast> return_statement
%type <print_ast> print_statement

%start program

%%

program:
	declaration_list procedure_definition_list
	{
	if(NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT((current_procedure != NULL), "Current procedure cannot be null");

		// Procedure * main_procedure = program_object.get_procedure("main");
		// program_object.set_procedure(current_procedure, get_line_number());
		program_object.global_list_in_proc_check();
	}
	}
;

declaration_list:
	procedure_declaration_list
	{
	if(NOT_ONLY_PARSE)
	{
		Symbol_Table * global_table = new Symbol_Table();
		program_object.set_global_table(*global_table);
	}
	}
|
	variable_declaration_list
	procedure_declaration_list
	{
	if(NOT_ONLY_PARSE)
	{
		Symbol_Table * global_table = $1;

		CHECK_INVARIANT((global_table != NULL), "Global declarations cannot be null");

		program_object.set_global_table(*global_table);
	}
	}
|
	procedure_declaration_list
	variable_declaration_list
	{
	if(NOT_ONLY_PARSE)
	{
		Symbol_Table * global_table = $2;

		CHECK_INVARIANT((global_table != NULL), "Global declarations cannot be null");

		program_object.set_global_table(*global_table);
	}
	}
;

procedure_declaration_list:
	procedure_declaration
	{
	if(NOT_ONLY_PARSE)
	{

	}
	}
|
	procedure_declaration_list procedure_declaration
	{
	if(NOT_ONLY_PARSE)
	{

	}
	}
;

procedure_declaration:
	return_data_type NAME '(' optional_argument_list ')' ';'
	{
	if(NOT_ONLY_PARSE)
	{
		// CHECK_INVARIANT()
		CHECK_INVARIANT(($2 != NULL), "Procedure name cannot be null");
		// CHECK_INVARIANT((*$2 == "main"), "Procedure name must be main in declaration");

		string proc_name = *$2;
		Procedure * procedure = new Procedure($1, proc_name, get_line_number());

		CHECK_INPUT ((program_object.variable_in_symbol_list_check(proc_name) == false),
			"Procedure name cannot be same as global variable", get_line_number());

		Symbol_Table * formal_table = $4;
		if(formal_table == NULL)
			formal_table = new Symbol_Table();
		procedure->set_formal_list(*formal_table);
		program_object.add_procedure(procedure, get_line_number());
	}
	}
|
	VOID NAME '(' optional_argument_list ')' ';'
	{
	if(NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT(($2 != NULL), "Procedure name cannot be null");
		// CHECK_INVARIANT((*$2 == "main"), "Procedure name must be main in declaration");

		string proc_name = *$2;
		Procedure * procedure = new Procedure(void_data_type, proc_name, get_line_number());
		// if(proc_name == "main")
		// 	current_procedure = procedure;

		CHECK_INPUT ((program_object.variable_in_symbol_list_check(proc_name) == false),
			"Procedure name cannot be same as global variable", get_line_number());

		Symbol_Table * formal_table = $4;
		if(formal_table == NULL)
			formal_table = new Symbol_Table();
		procedure->set_formal_list(*formal_table);
		program_object.add_procedure(procedure, get_line_number());
	}
	}
;

return_data_type:
	INTEGER
	{
	if(NOT_ONLY_PARSE)
	{
		$$ = int_data_type;
	}
	}
|
	FLOAT
	{
	if(NOT_ONLY_PARSE)
	{
		$$ = double_data_type;
	}
	}
;

procedure_definition:
	NAME '(' optional_argument_list ')'
	{
	if(NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT(($1 != NULL), "Procedure name cannot be null");
		string proc_name = *$1;
		current_procedure = program_object.get_procedure(proc_name);
		CHECK_INPUT(current_procedure != NULL, "Procedure corresponding to the name is not found", -1);
	}
	}

	'{' optional_variable_declaration_list
	{
	if(NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT((current_procedure != NULL), "Procedure prototype cannot be null");

		Symbol_Table * local_table = $7;

		if (local_table == NULL)
			local_table = new Symbol_Table();

		current_procedure->set_local_list(*local_table);

	}
	}

	statement_list return_statement '}'
	{
	if(NOT_ONLY_PARSE)
	{
		Sequence_Ast* seq = $9;
		CHECK_INVARIANT((current_procedure != NULL), "Current procedure cannot be null");
		CHECK_INVARIANT((seq != NULL), "statement list cannot be null");

		Return_Ast * return_stmt = $10;
		seq->ast_push_back(return_stmt);
		current_procedure->set_sequence_ast(*seq);
	}
	}
;

procedure_definition_list:
	procedure_definition
	{
	if(NOT_ONLY_PARSE)
	{
		// if(program_object)
	}
	}
|
	procedure_definition_list procedure_definition
	{
	if(NOT_ONLY_PARSE)
	{
		// if(program_object)
	}
	}
	;


optional_argument_list:
	/*empty*/
	{
	if(NOT_ONLY_PARSE)
	{
		$$ = NULL;
	}
	}
|
	argument_declaration_list
	{
	if(NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT(($1 != NULL), "Function Argument list cannot be null here");
		$$ = $1;
	}
	}
;

argument_declaration:
	return_data_type NAME
	{
	if(NOT_ONLY_PARSE)
	{
		$$ = new Symbol_Table_Entry(*$2, $1, get_line_number());
	}
	}
;

argument_declaration_list:
	argument_declaration
	{
	if(NOT_ONLY_PARSE)
	{
		Symbol_Table *argument_list = new Symbol_Table();
		Symbol_Table_Entry * decl = $1;

		CHECK_INVARIANT((decl != NULL), "Argument cannot be null");

		string decl_name = decl->get_variable_name();
		CHECK_INPUT ((program_object.variable_in_proc_map_check(decl_name) == false),
				"Procedure name cannot be same as formal parameter name", get_line_number());

		if(current_procedure != NULL)
		{
			CHECK_INPUT((current_procedure->get_proc_name() != decl_name),
				"Procedure name cannot be same as the formal parameter name", get_line_number());
		}

		// CHECK_INPUT((program_object.variable_in_symbol_list_check(decl_name) == false),
		// 			"Formal parameter and local variable name cannot be same", get_line_number());

		argument_list->push_symbol(decl);

		$$ = argument_list;
	}
	}
|
	argument_declaration_list ',' argument_declaration
	{
	if(NOT_ONLY_PARSE)
	{
		Symbol_Table *argument_list = $1;
		Symbol_Table_Entry * decl = $3;

		CHECK_INVARIANT((decl != NULL), "Argument cannot be null");
		CHECK_INVARIANT((argument_list != NULL), "argument_list cannot be null");

		string decl_name = decl->get_variable_name();
		CHECK_INPUT ((program_object.variable_in_proc_map_check(decl_name) == false),
				"Procedure name cannot be same as formal parameter name", get_line_number());

		if(current_procedure != NULL)
		{
			CHECK_INPUT((current_procedure->get_proc_name() != decl_name),
				"Procedure name cannot be same as the formal parameter name", get_line_number());
		}

		CHECK_INPUT((argument_list->variable_in_formal_list_check(decl_name) == false),
					"Formal Parameter declared twice", get_line_number());

		// CHECK_INPUT((program_object.variable_in_symbol_list_check(decl_name) == false),
		// 			"Formal parameter and local variable name cannot be same", get_line_number());

		argument_list->push_symbol(decl);

		$$ = argument_list;
	}
	}
;

optional_variable_declaration_list:
	{
	if(NOT_ONLY_PARSE)
	{
		$$ = NULL;
	}
	}
|
	variable_declaration_list
	{
	if(NOT_ONLY_PARSE)
	{

		CHECK_INVARIANT(($1 != NULL), "Declaration statement list cannot be null here");
		$$ = $1;

	}
	}
;


variable_declaration_list:
	variable_declaration
	{
	if(NOT_ONLY_PARSE)
	{


		Symbol_Table * decl_list = new Symbol_Table();
		list<Symbol_Table_Entry*> * symbol_entry_list = $1;

		CHECK_INVARIANT((symbol_entry_list != NULL), "Non-terminal declaration statement cannot be null");

		Symbol_Table_Entry * decl_stmt;
		string decl_name;
		for(auto it = symbol_entry_list->begin(); it != symbol_entry_list->end(); it++)
		{
			decl_stmt = *it;
			CHECK_INVARIANT((decl_stmt != NULL), "Non-terminal declaration statement cannot be null");

			string decl_name = decl_stmt->get_variable_name();
			CHECK_INPUT ((program_object.variable_in_proc_map_check(decl_name) == false),
					"Variable name cannot be same as the procedure name", get_line_number());

			if(current_procedure != NULL)
			{
				CHECK_INPUT((current_procedure->get_proc_name() != decl_name),
					"Variable name cannot be same as procedure name", get_line_number());
				CHECK_INPUT((current_procedure->variable_in_formal_list_check(decl_name) == false),
					"Formal parameter and local variable name cannot be same", get_line_number());
			}

			CHECK_INPUT((decl_list->variable_in_symbol_list_check(decl_name) == false),
					"Variable is declared twice", get_line_number());

			decl_list->push_symbol(decl_stmt);
		}

		$$ = decl_list;
	}
	}
|
	variable_declaration_list variable_declaration
	{
	if(NOT_ONLY_PARSE)
	{
		// if declaration is local then no need to check in global list
		// if declaration is global then this list is global list

		list<Symbol_Table_Entry*> *symbol_entry_list = $2;
		Symbol_Table * decl_list = $1;

		CHECK_INVARIANT((symbol_entry_list != NULL), "The declaration statement cannot be null");
		CHECK_INVARIANT((decl_list != NULL), "The declaration statement list cannot be null");

		Symbol_Table_Entry * decl_stmt;
		string decl_name;
		for(auto it = symbol_entry_list->begin(); it != symbol_entry_list->end(); it++)
		{
			decl_stmt = *it;
			decl_name = decl_stmt->get_variable_name();
			CHECK_INPUT((program_object.variable_in_proc_map_check(decl_name) == false),
				"Procedure name cannot be same as the variable name", get_line_number());
			if(current_procedure != NULL)
			{
				CHECK_INPUT((current_procedure->get_proc_name() != decl_name),
					"Variable name cannot be same as procedure name", get_line_number());
				CHECK_INPUT((current_procedure->variable_in_formal_list_check(decl_name) == false),
					"Formal parameter and local variable name cannot be same", get_line_number());
			}

			CHECK_INPUT((decl_list->variable_in_symbol_list_check(decl_name) == false),
					"Variable is declared twice", get_line_number());

			decl_list->push_symbol(decl_stmt);
		}
		$$ = decl_list;
	}
	}
;

variable_declaration:
	declaration ';'
	{
	if(NOT_ONLY_PARSE)
	{
		pair<Data_Type, list<string> * > * decl = $1;

		CHECK_INVARIANT((decl != NULL), "Declaration cannot be null");

		Data_Type type = decl->first;
		list<string> decls = *(decl->second);
		list<Symbol_Table_Entry*> * symbol_entry_list = new list<Symbol_Table_Entry*>();
		Symbol_Table_Entry * decl_entry;
		for ( auto it = decls.begin(); it != decls.end(); ++it )
		{
			decl_entry = new Symbol_Table_Entry(*it, type, get_line_number());
			symbol_entry_list->push_front(decl_entry);
		}

		$$ = symbol_entry_list;

	}
	}
;

declaration:
	return_data_type name_list
	{
	if(NOT_ONLY_PARSE)
	{

		CHECK_INVARIANT(($2 != NULL), "Name cannot be null");

		list<string> * name_list = $2;
		Data_Type type = $1;

		pair<Data_Type, list<string>* > * declar =
				new pair<Data_Type, list<string>* >(type, name_list);

		$$ = declar;

	}
	}
;

name_list:
	NAME
	{
	if(NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT(($1 != NULL), "Name cannot be null");
		list<string> * var_names = new list<string>();
		var_names->push_front(*$1);
		$$ = var_names;

	}
	}
|
	name_list ',' NAME
	{
	if(NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT(($1 != NULL) && ($3 != NULL),
			"Name or Name_list cannot be null");
		list<string> * var_names = $1;
		var_names->push_front(*$3);
		$$ = var_names;

	}
	}
;


statement_list:
	{
	if(NOT_ONLY_PARSE)
	{
		$$ = new Sequence_Ast(get_line_number());
	}
	}
|
	statement_list statement
	{
	if(NOT_ONLY_PARSE)
	{

		Sequence_Ast * seq = $1;
		Ast * stmt = $2;

		CHECK_INVARIANT((seq != NULL), "The statement list cannot be null");
		CHECK_INVARIANT((stmt != NULL), "The assignment statement cannot be null");

		seq->ast_push_back(stmt);
		$$ = seq;
	}
	}
;

return_statement:
	RETURN ';'
	{
	if(NOT_ONLY_PARSE)
	{
		$$ = new Return_Ast(NULL, current_procedure->get_proc_name(), get_line_number());
	}
	}
|
	RETURN arith_expression ';'
	{
	if(NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT(($2 != NULL), "return argument can't be NULL")
		$$ = new Return_Ast($2, current_procedure->get_proc_name(), get_line_number());
	}
	}
;

print_statement:
	PRINT '(' STRING ')' ';'
	{
	if(NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT(($3 != NULL),"String can't be null");
		String_Ast* str_ast = new String_Ast(*$3, "string", get_line_number());
		program_object.add_string(str_ast);
		$$ = new Print_Ast(str_ast);
	}
	}
|
	PRINT '(' arith_expression ')' ';'
	{
	if(NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT(($3 != NULL),"String can't be null");
		$$ = new Print_Ast($3);
	}
	}
;

other_statement:
	assignment_statement
	{
	if(NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT(($1 != NULL), "assignment_statement can't be null");
		$$ = $1;
	}
	}
|
	'{' statement_list '}'
	{
	if(NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT(($2 != NULL), "statement_list can't be null");
		$$ = $2;
	}
	}
|
	do_while_statement
	{
	if(NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT(($1 != NULL), "do_while_statement can't be null");
		$$ = $1;
	}
	}
|
	function_call_statement
	{
	if(NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT(($1 != NULL), "function_call_statement can't be null");
		$$ = $1;
	}
	}
|
	return_statement
	{
	if(NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT(($1 != NULL), "return_statement can't be null");
		$$ = $1;
	}
	}
|
	print_statement
	{
	if(NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT(($1 != NULL), "print_statement can't be null");
		$$ = $1;
	}
	}
;

statement:
	IF '(' bool_expression ')' statement
	ELSE statement
	{
	if(NOT_ONLY_PARSE)
	{
		Ast* cond = $3;
		Ast* then_part = $5;
		Ast* else_part = $7;
		CHECK_INVARIANT((cond != NULL) && (then_part != NULL) &&
				(else_part != NULL), "statement can't be null");
		$$ = new Selection_Statement_Ast(cond, then_part, else_part, get_line_number());
		//$$->check_ast();
	}
	}
|
	IF '(' bool_expression ')' statement %prec THEN
	{
	if(NOT_ONLY_PARSE)
	{
		Ast* cond = $3;
		Ast* then_part = $5;
		Ast* else_part = new Sequence_Ast(get_line_number());
		CHECK_INVARIANT((cond != NULL) && (then_part != NULL) &&
			(else_part != NULL), "statement can't be null");
		$$ = new Selection_Statement_Ast(cond, then_part, else_part, get_line_number());
	}
	}
|
	other_statement
	{
	if(NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT(($1 != NULL), "other_statement can't be null");
		$$ = $1;
	}
	}
|
	while_matched_statement
	{
	if(NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT(($1 != NULL), "while_statement can't be null");
		$$ = $1;
	}
	}
|
	for_matched_statement
	{
	if(NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT(($1 != NULL), "for_statement can't be null");
		$$ = $1;
	}
	}
;

while_matched_statement:
	WHILE '(' bool_expression ')' statement
	{
	if(NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT((($3 != NULL) && ($5 != NULL)),
			"do while cond and do statement block cannot be null");
		$$ = new Iteration_Statement_Ast($3, $5, get_line_number(), false);
		//$$->check_ast();
	}
	}
;

for_matched_statement:
	FOR '(' for_statement ';' bool_expression ';' for_statement ')' statement
	{
	if(NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT((($3 != NULL) && ($5 != NULL) && ($7 != NULL) && ($9 != NULL)),
			"for init, cond, increment and statement block cannot be null");
		Sequence_Ast* forLoop = new Sequence_Ast(get_line_number());
		Ast* init = $3;
		forLoop->ast_push_back(init);
		Sequence_Ast* body = new Sequence_Ast(get_line_number());
		body->ast_push_back($9);
		body->ast_push_back($7);
		Ast* loop = new Iteration_Statement_Ast($5, body, get_line_number(), false);
		forLoop->ast_push_back(loop);
		$$ = forLoop;
	}
	}
;


do_while_statement:
	DO statement WHILE '(' bool_expression ')' ';'
	{
	if(NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT((($2 != NULL) && ($5 != NULL)),
			"do while cond and do statement block cannot be null");
		$$ = new Iteration_Statement_Ast($5, $2, get_line_number(), true);
		//$$->check_ast();
	}
	}
;

for_statement:
	/*empty*/
	{
	if(NOT_ONLY_PARSE)
	{
		$$ = new Sequence_Ast(get_line_number());
	}
	}
|
	 variable ASSIGN arith_expression
	{
	if(NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT((($1 != NULL) && ($3 != NULL)), "lhs/rhs cannot be null");
		Ast* assign_ast = new Assignment_Ast($1, $3, get_line_number());
		$$ = assign_ast;
	}
	}
;

// Make sure to call check_ast in assignment_statement and arith_expression
// Refer to error_display.hh for displaying semantic errors if any
assignment_statement:
	variable ASSIGN arith_expression ';'
	{
	if (NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT((($1 != NULL) && ($3 != NULL)), "lhs/rhs cannot be null");

		Ast* lhs = $1;
		Ast* rhs = $3;
		Ast* assign_ast = new Assignment_Ast(lhs, rhs, get_line_number());
		//assign_ast->check_ast();
		$$ = assign_ast;
	}
	}
|
	variable ASSIGN function_call_statement
	{
	if (NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT((($1 != NULL) && ($3 != NULL)), "lhs/rhs cannot be null");

		Ast* lhs = $1;
		Ast* rhs = $3;
		Ast* assign_ast = new Assignment_Ast(lhs, rhs, get_line_number());
		$$ = assign_ast;
	}
	}
;

function_call_statement:
	NAME '(' optional_function_argument_list ')' ';'
	{
	if(NOT_ONLY_PARSE)
	{
		string fn_name = *$1;
		// CHECK_INVARIANT()
		Call_Ast * function_call = new Call_Ast(fn_name, get_line_number());

		list<Ast*> *argument_list = $3;
		if(argument_list == NULL)
			argument_list = new list<Ast*>();
		function_call->set_actual_param_list(*argument_list);
		$$ = function_call;
	}
	}
;

function_argument_list:
	arith_expression
	{
	if(NOT_ONLY_PARSE)
	{
		list<Ast*> *argument_list = new list<Ast*>();
		CHECK_INVARIANT(($1 != NULL), "Argument of a function can't be null");
		argument_list->push_front($1);
		$$ = argument_list;
	}
	}
|
	function_argument_list ',' arith_expression
	{
	if(NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT(($1 != NULL),"argument_list of a function can't be null");
		CHECK_INVARIANT(($3 != NULL), "Argument of a function can't be null");
		list<Ast*> * argument_list = $1;
		argument_list->push_front($3);
		$$ = argument_list;
	}
	}
;

optional_function_argument_list:
	/*empty*/
	{
	if(NOT_ONLY_PARSE)
	{
		$$ = NULL;
	}
	}
|
	function_argument_list
	{
	if(NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT(($1 != NULL), "function_argument_list cannot be null");
		$$ = $1;
	}
	}
;

relational_expression:
	operand LT operand
	{
	if (NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT((($1 != NULL) && ($3 != NULL)), "lhs/rhs cannot be null");
		Relational_Op rop = less_than;
		$$ = new Relational_Expr_Ast($1, rop, $3, get_line_number());
		//$$->check_ast();
	}
	}
|
	operand LE operand
	{
	if (NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT((($1 != NULL) && ($3 != NULL)), "lhs/rhs cannot be null");
		Relational_Op rop = less_equalto;
		$$ = new Relational_Expr_Ast($1, rop, $3, get_line_number());
		//$$->check_ast();
	}
	}
|
	operand GT operand
	{
	if (NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT((($1 != NULL) && ($3 != NULL)), "lhs/rhs cannot be null");
		Relational_Op rop = greater_than;
		$$ = new Relational_Expr_Ast($1, rop, $3, get_line_number());
		//$$->check_ast();
	}
	}
|
	operand GE operand
	{
	if (NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT((($1 != NULL) && ($3 != NULL)), "lhs/rhs cannot be null");
		Relational_Op rop = greater_equalto;
		$$ = new Relational_Expr_Ast($1, rop, $3, get_line_number());
		//$$->check_ast();
	}
	}
|
	operand EQ operand
	{
	if (NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT((($1 != NULL) && ($3 != NULL)), "lhs/rhs cannot be null");
		Relational_Op rop = equalto;
		$$ = new Relational_Expr_Ast($1, rop, $3, get_line_number());
		//$$->check_ast();
	}
	}
|
	operand NE operand
	{
	if (NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT((($1 != NULL) && ($3 != NULL)), "lhs/rhs cannot be null");
		Relational_Op rop = not_equalto;
		$$ = new Relational_Expr_Ast($1, rop, $3, get_line_number());
		//$$->check_ast();
	}
	}
;

// check_ast in boolean_expr with both rhs and lhs
bool_expression:
	NOT bool_expression
	{
	if (NOT_ONLY_PARSE)
	{
		Boolean_Op bop = boolean_not;
		CHECK_INVARIANT(($2 != NULL), "boolean_expr cannot be null");
		$$ = new Boolean_Expr_Ast(NULL, bop, $2, get_line_number());
	}
	}
|
	bool_expression AND bool_expression
	{
	if (NOT_ONLY_PARSE)
	{
		Boolean_Op bop = boolean_and;
		CHECK_INVARIANT((($1 != NULL) && ($3 != NULL)), "boolean_expr lhs/rhs cannot be null");
		$$ = new Boolean_Expr_Ast($1, bop, $3, get_line_number());
	}
	}
|
	bool_expression OR bool_expression
	{
	if (NOT_ONLY_PARSE)
	{
		Boolean_Op bop = boolean_or;
		CHECK_INVARIANT((($1 != NULL) && ($3 != NULL)), "boolean_expr lhs/rhs cannot be null");
		$$ = new Boolean_Expr_Ast($1, bop, $3, get_line_number());
	}
	}
|
	'(' bool_expression ')'
	{
	if (NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT(($2 != NULL), "boolean_expr cannot be null");
		$$ = $2;
	}
	}
|
	relational_expression
	{
	if (NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT(($1 != NULL), "Relational_Expr cannot be null");
		$$ = $1;
	}
	}
;


arith_expression:
		operand '+' operand
		{
		if (NOT_ONLY_PARSE)
		{
			CHECK_INVARIANT((($1 != NULL) && ($3 != NULL)), "lhs/rhs cannot be null");
			$$ = new Plus_Ast($1, $3, get_line_number());
			$$->check_ast();
		}
		}
|
		operand '-' operand
		{
		if (NOT_ONLY_PARSE)
		{
			CHECK_INVARIANT((($1 != NULL) && ($3 != NULL)), "lhs/rhs cannot be null");
			$$ = new Minus_Ast($1, $3, get_line_number());
			$$->check_ast();
		}
		}
|
		operand '*' operand
		{
		if (NOT_ONLY_PARSE)
		{
			CHECK_INVARIANT((($1 != NULL) && ($3 != NULL)), "lhs/rhs cannot be null");
			$$ = new Mult_Ast($1, $3, get_line_number());
			$$->check_ast();
		}
		}
|
		operand '/' operand
		{
		if (NOT_ONLY_PARSE)
		{
			CHECK_INVARIANT((($1 != NULL) && ($3 != NULL)), "lhs/rhs cannot be null");
			$$ = new Divide_Ast($1, $3, get_line_number());
			$$->check_ast();
		}
		}
|
		'-' operand %prec UMINUS
		{
		if (NOT_ONLY_PARSE)
		{
			CHECK_INVARIANT(($2 != NULL), "lhs of -E cannot be null");
			$$ = new UMinus_Ast($2 ,NULL, get_line_number());
		}
		}
|
		'(' operand ')'
		{
		if (NOT_ONLY_PARSE)
		{
			CHECK_INVARIANT(($2 != NULL), "Operand in a bracketed expression cannot be null");
			$$ = $2;
		}
		}
|
		expression_term
		{
		if (NOT_ONLY_PARSE)
		{
			CHECK_INVARIANT(($1 != NULL), "expression_term cannot be null");
			$$ = $1;
		}
		}
|
		bool_expression '?' operand ':' operand %prec TERNARY_COND
		{
		if(NOT_ONLY_PARSE)
		{
			CHECK_INVARIANT(($1 != NULL) && ($3 != NULL) && ($5 != NULL),
				"cond/lhs/rhs cannot be null");
			$$ = new Conditional_Operator_Ast($1, $3, $5, get_line_number());
			$$->check_ast();
		}
		};

operand:
	arith_expression
	{
	if (NOT_ONLY_PARSE)
	{
		// ADD CODE HERE
		CHECK_INVARIANT(($1 != NULL), "arith_expression cannot be null");
		$$ = $1;
	}
	}
;

expression_term:
	variable
	{
	if (NOT_ONLY_PARSE)
	{
		//ADD CODE HERE
		CHECK_INVARIANT(($1 != NULL), "Variable cannot be null");
		$$ = $1;
	}
	}
|
	constant
	{
	if (NOT_ONLY_PARSE)
	{
		//ADD CODE HERE
		CHECK_INVARIANT(($1 != NULL), "constant cannot be null");
		$$ = $1;

	}
	}
;

variable:
	NAME
	{
	if (NOT_ONLY_PARSE)
	{
		Symbol_Table_Entry * var_table_entry;

		CHECK_INVARIANT(($1 != NULL), "Variable name cannot be null");

		if (current_procedure->variable_in_symbol_list_check(*$1)
			|| current_procedure->variable_in_formal_list_check(*$1))
			 var_table_entry = &(current_procedure->get_symbol_table_entry(*$1));

		else if (program_object.variable_in_symbol_list_check(*$1))
			var_table_entry = &(program_object.get_symbol_table_entry(*$1));


		else
			CHECK_INPUT_AND_ABORT(CONTROL_SHOULD_NOT_REACH, "Variable has not been declared", get_line_number());

		$$ = new Name_Ast(*$1, *var_table_entry, get_line_number());
		delete $1;
	}
	}
;

constant:
	INTEGER_NUMBER
	{
	if (NOT_ONLY_PARSE)
	{
		int number = $1;
		Ast* number_ast = new Number_Ast<int>(number, int_data_type, get_line_number());
		$$ = number_ast;
	}
	}
|
	DOUBLE_NUMBER
	{
	if (NOT_ONLY_PARSE)
	{
		double number = $1;

		Ast* number_ast = new Number_Ast<double>(number, double_data_type, get_line_number());
		$$ = number_ast;
	}
	}
;
