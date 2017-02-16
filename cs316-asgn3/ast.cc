#include<iostream>
#include<fstream>
#include<typeinfo>

using namespace std;

#include"common-classes.hh"
#include"error-display.hh"
#include"user-options.hh"
#include"local-environment.hh"
#include"symbol-table.hh"
#include"ast.hh"
#include"procedure.hh"
#include"program.hh"

int Ast::labelCounter=0;

Ast::Ast()
{}

Ast::~Ast()
{}

bool Ast::check_ast()
{
	stringstream msg;
	msg << "No check_ast() function for " << typeid(*this).name();
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, msg.str());
}

Data_Type Ast::get_data_type()
{
	stringstream msg;
	msg << "No get_data_type() function for " << typeid(*this).name();
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, msg.str());
}

Symbol_Table_Entry & Ast::get_symbol_entry()
{
	stringstream msg;
	msg << "No get_symbol_entry() function for " << typeid(*this).name();
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, msg.str());
}

bool Ast::is_value_zero()
{
	stringstream msg;
	msg << "No is_value_zero() function for " << typeid(*this).name();
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, msg.str());
}

void Ast::set_data_type(Data_Type dt)
{
	stringstream msg;
	msg << "No set_data_type() function for " << typeid(*this).name();
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, msg.str());
}

////////////////////////////////////////////////////////////////

Assignment_Ast::Assignment_Ast(Ast * temp_lhs, Ast * temp_rhs, int line)
{
	//ADD CODE HERE
	this->lhs = temp_lhs;
	this->rhs = temp_rhs;
	this->lineno = line;
	this->ast_num_child = binary_arity;
	if(check_ast()){
		this->node_data_type = lhs->get_data_type();
	}
	else{
		this->node_data_type = void_data_type;
	}
}

Assignment_Ast::~Assignment_Ast()
{
	//ADD CODE HERE
	delete lhs;
	delete rhs;
}

bool Assignment_Ast::check_ast()
{
	CHECK_INVARIANT((rhs != NULL), "Rhs of Assignment_Ast cannot be null");
	CHECK_INVARIANT((lhs != NULL), "Lhs of Assignment_Ast cannot be null");

	// use typeid(), get_data_type()
	//ADD CODE HERE
	if(lhs->get_data_type() == rhs->get_data_type())
		return true;

	CHECK_INPUT(CONTROL_SHOULD_NOT_REACH,
		"Assignment statement data type not compatible", lineno);
}

void Assignment_Ast::print(ostream & file_buffer)
{
	//ADD CODE HERE
	file_buffer << endl << AST_SPACE << "Asgn:" << endl;
	file_buffer << AST_NODE_SPACE << "LHS (";
	lhs->print(file_buffer);
	file_buffer << ")" << endl;
	file_buffer << AST_NODE_SPACE << "RHS (";
	rhs->print(file_buffer);
	file_buffer << ")";
}

/////////////////////////////////////////////////////////////////

Name_Ast::Name_Ast(string & name, Symbol_Table_Entry & var_entry, int line)
{
	//ADD CODE HERE
	this->variable_symbol_entry = &var_entry;
	this->node_data_type = variable_symbol_entry->get_data_type();
	this->ast_num_child = zero_arity;
	this->lineno = line;
	CHECK_INVARIANT((variable_symbol_entry->get_variable_name() == name),
		"Variable's symbol entry is not matching its name");
}

Name_Ast::~Name_Ast()
{}

Data_Type Name_Ast::get_data_type()
{
	// refer to functions for Symbol_Table_Entry
	//ADD CODE HERE
	return this->variable_symbol_entry->get_data_type();
}

Symbol_Table_Entry & Name_Ast::get_symbol_entry()
{
	//ADD CODE HERE
	return *(this->variable_symbol_entry);
}

void Name_Ast::set_data_type(Data_Type dt)
{
	//ADD CODE HERE
	this->variable_symbol_entry->set_data_type(dt);
}

void Name_Ast::print(ostream & file_buffer)
{
	//ADD CODE HERE
	file_buffer << "Name : " << variable_symbol_entry->get_variable_name();
}

///////////////////////////////////////////////////////////////////////////////

template <class DATA_TYPE>
Number_Ast<DATA_TYPE>::Number_Ast(DATA_TYPE number, Data_Type constant_data_type, int line)
{
	// use Ast_arity from ast.hh
	//ADD CODE HERE
	this->constant = number;
	this->node_data_type = constant_data_type;
	this->lineno = line;
	this->ast_num_child = zero_arity;
}

template <class DATA_TYPE>
Number_Ast<DATA_TYPE>::~Number_Ast()
{}

template <class DATA_TYPE>
Data_Type Number_Ast<DATA_TYPE>::get_data_type()
{
	//ADD CODE HERE
	return this->node_data_type;
}

template <class DATA_TYPE>
void Number_Ast<DATA_TYPE>::set_data_type(Data_Type dt)
{
	//ADD CODE HERE
	this->node_data_type = dt;
}

template <class DATA_TYPE>
bool Number_Ast<DATA_TYPE>::is_value_zero()
{
	//ADD CODE HERE
	return (constant == 0);
}

template <class DATA_TYPE>
void Number_Ast<DATA_TYPE>::print(ostream & file_buffer)
{
	//ADD CODE HERE
	file_buffer << "Num : " << constant;
}

///////////////////////////////////////////////////////////////////////////////

Data_Type Arithmetic_Expr_Ast::get_data_type()
{
	//ADD CODE HERE
	return this->node_data_type;
}

void Arithmetic_Expr_Ast::set_data_type(Data_Type dt)
{
	//ADD CODE HERE
	this->node_data_type = dt;
}

bool Arithmetic_Expr_Ast::check_ast()
{
	// use get_data_type(), typeid()
	//ADD CODE HERE
	if (lhs->get_data_type() == rhs->get_data_type())
		return true;

	CHECK_INPUT(CONTROL_SHOULD_NOT_REACH, "Arithmetic statement data type not compatible", lineno);
}

Arithmetic_Expr_Ast::~Arithmetic_Expr_Ast()
{
	delete lhs;
	delete rhs;
}

/////////////////////////////////////////////////////////////////////

Plus_Ast::Plus_Ast(Ast * l, Ast * r, int line)
{
	// set arity and data type
	//ADD CODE HERE
	this->lhs = l;
	this->rhs = r;
	this->ast_num_child = binary_arity;
	this->lineno = line;
	if(l->get_data_type()==r->get_data_type()){
		this->node_data_type = l->get_data_type();
	}
	else{
		this->node_data_type = void_data_type;
	}
}

void Plus_Ast::print(ostream & file_buffer)
{
	//ADD CODE HERE
	file_buffer << endl << AST_NODE_SPACE << "Arith: PLUS" << endl;
	file_buffer << AST_SUB_NODE_SPACE << "LHS (";
	lhs->print(file_buffer);
	file_buffer << ")" << endl;
	file_buffer << AST_SUB_NODE_SPACE << "RHS (";
	rhs->print(file_buffer);
	file_buffer << ")";
}

/////////////////////////////////////////////////////////////////

Minus_Ast::Minus_Ast(Ast * l, Ast * r, int line)
{
	//ADD CODE HERE
	this->lhs = l;
	this->rhs = r;
	this->ast_num_child = binary_arity;
	this->lineno = line;
	if(l->get_data_type()==r->get_data_type()){
		this->node_data_type = l->get_data_type();
	}
	else{
		this->node_data_type = void_data_type;
	}
}

void Minus_Ast::print(ostream & file_buffer)
{
	//ADD CODE HERE
	file_buffer << endl << AST_NODE_SPACE << "Arith: MINUS" << endl;
	file_buffer << AST_SUB_NODE_SPACE << "LHS (";
	lhs->print(file_buffer);
	file_buffer << ")" << endl;
	file_buffer << AST_SUB_NODE_SPACE << "RHS (";
	rhs->print(file_buffer);
	file_buffer << ")";
}

//////////////////////////////////////////////////////////////////

Mult_Ast::Mult_Ast(Ast * l, Ast * r, int line)
{
	//ADD CODE HERE
	this->lhs = l;
	this->rhs = r;
	this->ast_num_child = binary_arity;
	this->lineno = line;
	if(l->get_data_type()==r->get_data_type()){
		this->node_data_type = l->get_data_type();
	}
	else{
		this->node_data_type = void_data_type;
	}
}

void Mult_Ast::print(ostream & file_buffer)
{
	//ADD CODE HERE
	file_buffer << endl << AST_NODE_SPACE << "Arith: MULT" << endl;
	file_buffer << AST_SUB_NODE_SPACE << "LHS (";
	lhs->print(file_buffer);
	file_buffer << ")" << endl;
	file_buffer << AST_SUB_NODE_SPACE << "RHS (";
	rhs->print(file_buffer);
	file_buffer << ")";
}

////////////////////////////////////////////////////////////////////

Divide_Ast::Divide_Ast(Ast * l, Ast * r, int line)
{
	//ADD CODE HERE
	this->lhs = l;
	this->rhs = r;
	this->ast_num_child = binary_arity;
	this->lineno = line;
	if(l->get_data_type()==r->get_data_type()){
		this->node_data_type = l->get_data_type();
	}
	else{
		this->node_data_type = void_data_type;
	}
}

void Divide_Ast::print(ostream & file_buffer)
{
	//ADD CODE HERE
	file_buffer << endl << AST_NODE_SPACE << "Arith: DIV" << endl;
	file_buffer << AST_SUB_NODE_SPACE << "LHS (";
	lhs->print(file_buffer);
	file_buffer << ")" << endl;
	file_buffer << AST_SUB_NODE_SPACE << "RHS (";
	rhs->print(file_buffer);
	file_buffer << ")";
}

//////////////////////////////////////////////////////////////////////

UMinus_Ast::UMinus_Ast(Ast * l, Ast * r, int line)
{
	//ADD CODE HERE
	this->lhs = l;
	this->rhs = NULL;
	this->ast_num_child = unary_arity;
	this->lineno = line;
	this->node_data_type = l->get_data_type();
}

void UMinus_Ast::print(ostream & file_buffer)
{
	//ADD CODE HERE
	file_buffer << endl << AST_NODE_SPACE << "Arith: UMINUS" << endl;
	file_buffer << AST_SUB_NODE_SPACE << "LHS (";
	lhs->print(file_buffer);
	file_buffer << ")";
}


template class Number_Ast<double>;
template class Number_Ast<int>;

Relational_Expr_Ast::Relational_Expr_Ast(Ast * lhs, Relational_Op rop, Ast * rhs, int line)
{
	this->lhs_condition = lhs;
	this->rhs_condition = rhs;
	this->rel_op = rop;
	this->lineno = line;
	this->ast_num_child = binary_arity;
	if(check_ast()){
		this->node_data_type = int_data_type;
	}
	else{
		this->node_data_type = void_data_type;
	}
}

Relational_Expr_Ast::~Relational_Expr_Ast()
{
	delete lhs_condition;
	delete rhs_condition;
}

Data_Type Relational_Expr_Ast::get_data_type()
{
	return this->node_data_type;
}

void Relational_Expr_Ast::set_data_type(Data_Type dt)
{
	this->node_data_type = dt;
}

bool Relational_Expr_Ast::check_ast()
{
	CHECK_INVARIANT((rhs_condition != NULL), "Rhs of Relational_Expr_Ast cannot be null");
	CHECK_INVARIANT((lhs_condition != NULL), "Lhs of Relational_Expr_Ast cannot be null");

	// use typeid(), get_data_type()
	//ADD CODE HERE
	if(lhs_condition->get_data_type() == rhs_condition->get_data_type())
	{
		return true;
	}

	CHECK_INPUT(CONTROL_SHOULD_NOT_REACH,
		"Relational statement data type not compatible", lineno);
}

void Relational_Expr_Ast::print(ostream & file_buffer)
{
	file_buffer << endl << AST_NODE_SPACE << "Condition: ";
	string rel_operator;
	switch(rel_op)
	{
	case less_equalto:
		rel_operator = "LE";
		break;
	case less_than:
		rel_operator = "LT";
		break;
	case greater_than:
		rel_operator = "GT";
		break;
	case greater_equalto:
		rel_operator = "GE";
		break;
	case equalto:
		rel_operator = "EQ";
		break;
	case not_equalto:
		rel_operator = "NE";
		break;
	}
	file_buffer << rel_operator << endl;
	file_buffer << AST_SUB_NODE_SPACE << "LHS (";
	lhs_condition->print(file_buffer);
	file_buffer << ")" << endl;
	file_buffer << AST_SUB_NODE_SPACE << "RHS (";
	rhs_condition->print(file_buffer);
	file_buffer << ")";
}

////////////////////////////////////////////////////////////////////////


Boolean_Expr_Ast::Boolean_Expr_Ast(Ast * lhs, Boolean_Op bop, Ast * rhs, int line)
{
	this->lhs_op = lhs;
	this->rhs_op = rhs;
	this->bool_op = bop;
	if (bop == boolean_not)
	{
		this->ast_num_child = unary_arity;
	}
	else{
		this->ast_num_child = binary_arity;
	}
	this->lineno = line;
	if(check_ast()){
		this->node_data_type = int_data_type;
	}
	else{
		this->node_data_type = void_data_type;
	}
}

Boolean_Expr_Ast::~Boolean_Expr_Ast()
{
	delete lhs_op;
	delete rhs_op;
}

Data_Type Boolean_Expr_Ast::get_data_type()
{
	return this->node_data_type;
}

void Boolean_Expr_Ast::set_data_type(Data_Type dt)
{
	this->node_data_type = dt;
}

bool Boolean_Expr_Ast::check_ast()
{
	CHECK_INVARIANT((rhs_op != NULL), "Rhs of Boolean_Expr_Ast cannot be null");
	CHECK_INVARIANT((lhs_op != NULL || ast_num_child == unary_arity), "Lhs of Boolean_Expr_Ast cannot be null");

	// use typeid(), get_data_type()
	//ADD CODE HERE
	if(ast_num_child == unary_arity && rhs_op->get_data_type() == int_data_type)
		return true;
	else if (lhs_op->get_data_type()==rhs_op->get_data_type() && lhs_op->get_data_type() == int_data_type)
		return true;

	CHECK_INPUT(CONTROL_SHOULD_NOT_REACH,
		"Boolean Expression data type not compatible", lineno);
}

void Boolean_Expr_Ast::print(ostream & file_buffer)
{
	file_buffer << endl << AST_NODE_SPACE << "Condition: ";
	string bool_operator;
	switch(bool_op)
	{
		case boolean_not:
			bool_operator="NOT";
			break;
		case boolean_or:
			bool_operator = "OR";
			break;
		case boolean_and:
			bool_operator = "AND";
			break;
	}
	file_buffer << bool_operator << endl;
	if(lhs_op != NULL)
	{
		file_buffer << AST_SUB_NODE_SPACE << "LHS (";
		lhs_op->print(file_buffer);
		file_buffer << ")" << endl;
	}
	file_buffer << AST_SUB_NODE_SPACE << "RHS (";
	rhs_op->print(file_buffer);
	file_buffer << ")";
}

//////////////////////////////////////////////////////////////////////



Selection_Statement_Ast::Selection_Statement_Ast(Ast * cond, Ast* then_part, Ast* else_part, int line)
{
	this->cond = cond;
	this->then_part = then_part;
	this->else_part = else_part;
	this->ast_num_child = binary_arity;
	this->lineno = line;
	if(check_ast()){
		this->node_data_type = int_data_type;
	}
	else{
		this->node_data_type = void_data_type;
	}
}

Selection_Statement_Ast::~Selection_Statement_Ast()
{
	delete cond;
	delete then_part;
	delete else_part;
}

Data_Type Selection_Statement_Ast::get_data_type()
{
	return this->node_data_type;
}

void Selection_Statement_Ast::set_data_type(Data_Type dt)
{
	this->node_data_type = dt;
}

bool Selection_Statement_Ast::check_ast()
{
	CHECK_INVARIANT((cond != NULL), "Condition of Selection_Statement_Ast cannot be null");
	CHECK_INVARIANT((then_part != NULL), "If part of Selection_Statement_Ast cannot be null");
}

void Selection_Statement_Ast::print(ostream & file_buffer)
{
	file_buffer << endl << AST_SPACE << "IF : " << endl;
	file_buffer << AST_SPACE << "CONDITION (";
	cond->print(file_buffer);
	file_buffer << ")" << endl;
	file_buffer << AST_SPACE << "THEN (";
	then_part->print(file_buffer);
	file_buffer << ")" << endl;
	file_buffer << AST_SPACE << "ELSE (";
	else_part->print(file_buffer);
	file_buffer << ")";
}

/////////////////////////////////////////////////////////////////////////////////


Iteration_Statement_Ast::Iteration_Statement_Ast(Ast * cond, Ast* body, int line, bool do_form)
{
	this->cond = cond;
	this->body = body;
	this->ast_num_child = unary_arity;
	this->lineno = line;
	this->is_do_form = do_form;
	this->node_data_type = cond->get_data_type();
}

Iteration_Statement_Ast::~Iteration_Statement_Ast()
{}

Data_Type Iteration_Statement_Ast::get_data_type()
{
	return this->node_data_type;
}

void Iteration_Statement_Ast::set_data_type(Data_Type dt)
{
	this->node_data_type = dt;
}

bool Iteration_Statement_Ast::check_ast()
{
	CHECK_INVARIANT((cond != NULL), "Condition of Selection_Statement_Ast cannot be null");
	CHECK_INVARIANT((body != NULL), "If part of Selection_Statement_Ast cannot be null");
}

void Iteration_Statement_Ast::print(ostream & file_buffer)
{
	file_buffer << endl << AST_SPACE << "WHILE : " << endl;
	file_buffer << AST_SPACE << "CONDITION (";
	cond->print(file_buffer);
	file_buffer << ")" << endl;
	file_buffer << AST_SPACE << "BODY (";
	body->print(file_buffer);
	file_buffer << ")";
}

///////////////////////////////////////////////////////////////////////////////////


Conditional_Operator_Ast::Conditional_Operator_Ast(Ast* cond, Ast* l, Ast* r, int line)
{
	this->cond = cond;
	this->lhs = l;
	this->rhs = r;
	this->ast_num_child = ternary_arity;
	this->lineno = line;
	//CHECK_INPUT(lhs->get_data_type()==rhs->get_data_type(),"Arithmetic statement data type not compatible",lineno);
	if (lhs->get_data_type()==rhs->get_data_type())
	{
		this->node_data_type = l->get_data_type();
	}
	else
	{
		this->node_data_type = void_data_type;
	}
}


void Conditional_Operator_Ast::print(ostream & file_buffer)
{
	file_buffer << endl << AST_NODE_SPACE << "Arith: Conditional" << endl;
	file_buffer << AST_SUB_NODE_SPACE << "COND (";
	cond->print(file_buffer);
	file_buffer << ")" << endl;
	file_buffer << AST_SUB_NODE_SPACE << "LHS (";
	lhs->print(file_buffer);
	file_buffer << ")" << endl;
	file_buffer << AST_SUB_NODE_SPACE << "RHS (";
	rhs->print(file_buffer);
	file_buffer << ")";
}

////////////////////////////////////////////////////////////////////////////////////


Sequence_Ast::Sequence_Ast(int line)
{
	this->lineno = line;
}

Sequence_Ast::~Sequence_Ast()
{}

void Sequence_Ast::ast_push_back(Ast * ast)
{
	statement_list.push_back(ast);
}

void Sequence_Ast::print(ostream & file_buffer)
{
	file_buffer << endl << SA_SPACE << "Sequence Ast:" << endl;
	for (auto it=statement_list.begin(); it != statement_list.end(); ++it)
	{
		(*it)->print(file_buffer);
	}
}
