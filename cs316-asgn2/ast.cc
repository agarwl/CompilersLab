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
#include"sequence-ast.hh"
#include"procedure.hh"
#include"program.hh"

int NUM_SPACES = 9;

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
	this->node_data_type = lhs->get_data_type();
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
	file_buffer << string(NUM_SPACES, ' ') << "Asgn:" << endl;
	NUM_SPACES += 3;
	file_buffer << string(NUM_SPACES, ' ') << "LHS (";
	lhs->print(file_buffer);
	file_buffer << ")" << endl;
	file_buffer << string(NUM_SPACES, ' ') << "RHS (";
	rhs->print(file_buffer);
	file_buffer << ")" << endl;
	NUM_SPACES -= 3;
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
	if (lhs->get_data_type()== rhs->get_data_type())
		return true;

	CHECK_INPUT(CONTROL_SHOULD_NOT_REACH, "Arithmetic statement data type not compatible", lineno);
}

/////////////////////////////////////////////////////////////////////

Plus_Ast::Plus_Ast(Ast * l, Ast * r, int line)
{
	// set arity and data type
	//ADD CODE HERE
	this->lhs = l;
	this->rhs = r;
	this->ast_num_child = binary_arity;
	this->node_data_type = l->get_data_type();
	this->lineno = line;
}

void Plus_Ast::print(ostream & file_buffer)
{
	//ADD CODE HERE
	file_buffer << endl << string(NUM_SPACES, ' ') << "Arith: PLUS" << endl;
	NUM_SPACES += 3;
	file_buffer << string(NUM_SPACES, ' ') << "LHS (";
	lhs->print(file_buffer);
	file_buffer << ")" << endl;
	file_buffer << string(NUM_SPACES, ' ') << "RHS (";
	rhs->print(file_buffer);
	file_buffer << ")";
	NUM_SPACES -= 3;
}

/////////////////////////////////////////////////////////////////

Minus_Ast::Minus_Ast(Ast * l, Ast * r, int line)
{
	//ADD CODE HERE
	this->lhs = l;
	this->rhs = r;
	this->ast_num_child = binary_arity;
	this->node_data_type = l->get_data_type();
	this->lineno = line;
}

void Minus_Ast::print(ostream & file_buffer)
{
	//ADD CODE HERE
	file_buffer << endl << string(NUM_SPACES, ' ') << "Arith: MINUS" << endl;
	NUM_SPACES += 3;
	file_buffer << string(NUM_SPACES, ' ') << "LHS (";
	lhs->print(file_buffer);
	file_buffer << ")" << endl;
	file_buffer << string(NUM_SPACES, ' ') << "RHS (";
	rhs->print(file_buffer);
	file_buffer << ")";
	NUM_SPACES -= 3;
}

//////////////////////////////////////////////////////////////////

Mult_Ast::Mult_Ast(Ast * l, Ast * r, int line)
{
	//ADD CODE HERE
	this->lhs = l;
	this->rhs = r;
	this->ast_num_child = binary_arity;
	this->node_data_type = l->get_data_type();
	this->lineno = line;
}

void Mult_Ast::print(ostream & file_buffer)
{
	//ADD CODE HERE
}

////////////////////////////////////////////////////////////////////

Divide_Ast::Divide_Ast(Ast * l, Ast * r, int line)
{
	//ADD CODE HERE
	this->lhs = l;
	this->rhs = r;
	this->ast_num_child = binary_arity;
	this->node_data_type = l->get_data_type();
	this->lineno = line;
}

void Divide_Ast::print(ostream & file_buffer)
{
	//ADD CODE HERE
}

//////////////////////////////////////////////////////////////////////

UMinus_Ast::UMinus_Ast(Ast * l, Ast * r, int line)
{
	//ADD CODE HERE
	this->lhs = l;
	this->rhs = NULL;
	this->ast_num_child = unary_arity;
	this->node_data_type = l->get_data_type();
	this->lineno = line;
}

void UMinus_Ast::print(ostream & file_buffer)
{
	//ADD CODE HERE
}


template class Number_Ast<double>;
template class Number_Ast<int>;
