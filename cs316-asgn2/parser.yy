%scanner scanner.h
%scanner-token-function d_scanner.lex()
%filenames parser
%parsefun-source parser.cc

%union
{

	pair<Data_Type, string> * decl;
	Symbol_Table * symbol_table;
	Symbol_Table_Entry * symbol_entry;
	Procedure * procedure;

	//ADD CODE HERE
	Sequence_Ast * sequence_ast;
	Ast * ast;
	int integer_value;
	std::string * string_value;
	double double_value;
};

//ADD TOKENS HERE
%token <integer_value> INTEGER_NUMBER
%token <string_value> NAME
%token RETURN INTEGER
%token ASSIGN VOID
%token <double_value> DOUBLE_NUMBER
%token FLOAT

%left '+' '-'
%left '*' '/'
%right UMINUS
%nonassoc '('

%type <symbol_table> optional_variable_declaration_list
%type <symbol_table> variable_declaration_list
%type <symbol_entry> variable_declaration
%type <decl> declaration
//ADD CODE HERE

%type <sequence_ast> statement_list
%type <ast> assignment_statement
%type <ast> variable
%type <ast> constant
%type <ast> operand
%type <ast> arith_expression
%type <ast> expression_term


%start program

%%

program:
	declaration_list procedure_definition
	{
	if (NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT((current_procedure != NULL), "Current procedure cannot be null");

		program_object.set_procedure(current_procedure, get_line_number());
		program_object.global_list_in_proc_check();
	}
	}
;

declaration_list:
	procedure_declaration
	{
	if (NOT_ONLY_PARSE)
	{
		Symbol_Table * global_table = new Symbol_Table();
		program_object.set_global_table(*global_table);
	}
	}
|
	variable_declaration_list
	procedure_declaration
	{
	if (NOT_ONLY_PARSE)
	{
		Symbol_Table * global_table = $1;

		CHECK_INVARIANT((global_table != NULL), "Global declarations cannot be null");

		program_object.set_global_table(*global_table);
	}
	}
|
	procedure_declaration
	variable_declaration_list
	{
	if (NOT_ONLY_PARSE)
	{
		Symbol_Table * global_table = $2;

		CHECK_INVARIANT((global_table != NULL), "Global declarations cannot be null");

		program_object.set_global_table(*global_table);
	}
	}
;

procedure_declaration:
	VOID NAME '(' ')' ';'
	{
	if (NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT(($2 != NULL), "Procedure name cannot be null");
		CHECK_INVARIANT((*$2 == "main"), "Procedure name must be main in declaration");
	}
	}
;

procedure_definition:
	NAME '(' ')'
	{
	if (NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT(($1 != NULL), "Procedure name cannot be null");
		CHECK_INVARIANT((*$1 == "main"), "Procedure name must be main");

		string proc_name = *$1;

		current_procedure = new Procedure(void_data_type, proc_name, get_line_number());

		CHECK_INPUT ((program_object.variable_in_symbol_list_check(proc_name) == false),
			"Procedure name cannot be same as global variable", get_line_number());
	}
	}

	'{' optional_variable_declaration_list
	{
	if (NOT_ONLY_PARSE)
	{

		CHECK_INVARIANT((current_procedure != NULL), "Current procedure cannot be null");

		Symbol_Table * local_table = $6;

		if (local_table == NULL)
			local_table = new Symbol_Table();

		current_procedure->set_local_list(*local_table);
	}
	}

	statement_list '}'
	{
	if (NOT_ONLY_PARSE)
	{
		Sequence_Ast* seq = $8;

		CHECK_INVARIANT((current_procedure != NULL), "Current procedure cannot be null");
		CHECK_INVARIANT((seq != NULL), "statement list cannot be null");

		current_procedure->set_sequence_ast(*seq);
	}
	}
;

optional_variable_declaration_list:
	{
	if (NOT_ONLY_PARSE)
	{
		$$ = NULL;
	}
	}
|
	variable_declaration_list
	{
	if (NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT(($1 != NULL), "Declaration statement list cannot be null here");

		$$ = $1;
	}
	}
;

variable_declaration_list:
	variable_declaration
	{
	if (NOT_ONLY_PARSE)
	{
		Symbol_Table_Entry * decl_stmt = $1;

		CHECK_INVARIANT((decl_stmt != NULL), "Non-terminal declaration statement cannot be null");

		string decl_name = decl_stmt->get_variable_name();
		CHECK_INPUT ((program_object.variable_proc_name_check(decl_name) == false),
				"Variable name cannot be same as the procedure name", get_line_number());

		if(current_procedure != NULL)
		{
			CHECK_INPUT((current_procedure->get_proc_name() != decl_name),
				"Variable name cannot be same as procedure name", get_line_number());
		}

		Symbol_Table * decl_list = new Symbol_Table();
		decl_list->push_symbol(decl_stmt);

		$$ = decl_list;
	}
	}
|
	variable_declaration_list variable_declaration
	{
	if (NOT_ONLY_PARSE)
	{
		// if declaration is local then no need to check in global list
		// if declaration is global then this list is global list

		Symbol_Table_Entry * decl_stmt = $2;
		Symbol_Table * decl_list = $1;

		CHECK_INVARIANT((decl_stmt != NULL), "The declaration statement cannot be null");
		CHECK_INVARIANT((decl_list != NULL), "The declaration statement list cannot be null");

		string decl_name = decl_stmt->get_variable_name();
		CHECK_INPUT((program_object.variable_proc_name_check(decl_name) == false),
			"Procedure name cannot be same as the variable name", get_line_number());
		if(current_procedure != NULL)
		{
			CHECK_INPUT((current_procedure->get_proc_name() != decl_name),
				"Variable name cannot be same as procedure name", get_line_number());
		}

		CHECK_INPUT((decl_list->variable_in_symbol_list_check(decl_name) == false),
				"Variable is declared twice", get_line_number());

		decl_list->push_symbol(decl_stmt);
		$$ = decl_list;
	}
	}
;

variable_declaration:
	declaration ';'
	{
	if (NOT_ONLY_PARSE)
	{
		pair<Data_Type, string> * decl = $1;

		CHECK_INVARIANT((decl != NULL), "Declaration cannot be null");

		Data_Type type = decl->first;
		string decl_name = decl->second;

		Symbol_Table_Entry * decl_entry = new Symbol_Table_Entry(decl_name, type, get_line_number());

		$$ = decl_entry;

	}
	}
;

declaration:
	INTEGER NAME
	{
	if (NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT(($2 != NULL), "Name cannot be null");

		string name = *$2;
		Data_Type type = int_data_type;

		pair<Data_Type, string> * declar = new pair<Data_Type, string>(type, name);

		$$ = declar;
	}
	}
|
	FLOAT NAME
	{
	if (NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT(($2 != NULL), "Name cannot be null");

		string name = *$2;
		Data_Type type = double_data_type;

		pair<Data_Type, string> * declar = new pair<Data_Type, string>(type, name);

		$$ = declar;
	}
	}
;


statement_list:
	{
	if (NOT_ONLY_PARSE)
	{
		$$ = NULL;
	}
	}
|
	statement_list assignment_statement
	{
	if (NOT_ONLY_PARSE)
	{

		Sequence_Ast * seq = $1;
		Ast * asnmt = $2;

		if (seq == NULL)
		{
			seq = new Sequence_Ast(get_line_number());
		}

		CHECK_INVARIANT((seq != NULL), "The statement list cannot be null");
		CHECK_INVARIANT((asnmt != NULL), "The assignment statement cannot be null");

		seq->ast_push_back(asnmt);
		$$ = seq;
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
		assign_ast->check_ast();
		$$ = assign_ast;
	}
	}
;

arith_expression:
		//ADD RELEVANT CODE ALONG WITH GRAMMAR RULES HERE
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
			// $$ = $1/$3;
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
                // SUPPORT binary +, -, *, / operations, unary -, and allow parenthesization
                // i.e. E -> (E)
                // Connect the rules with the remaining rules given below
;

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

		if (current_procedure->variable_in_symbol_list_check(*$1))
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
