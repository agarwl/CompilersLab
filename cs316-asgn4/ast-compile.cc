#include <iostream>
#include <fstream>
#include <typeinfo>

using namespace std;

#include "common-classes.hh"
#include "error-display.hh"
#include "user-options.hh"
#include "icode.hh"
#include "reg-alloc.hh"
#include "symbol-table.hh"
#include "ast.hh"
#include "procedure.hh"
#include "program.hh"

Code_For_Ast & Ast::create_store_stmt(Register_Descriptor * store_register)
{
	stringstream msg;
	msg << "No create_store_stmt() function for " << typeid(*this).name();
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, msg.str());
}

void Ast::print_assembly()
{
	stringstream msg;
	msg << "No print_assembly() function for " << typeid(*this).name();
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, msg.str());
}

void Ast::print_icode()
{
	stringstream msg;
	msg << "No print_icode() function for " << typeid(*this).name();
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, msg.str());
}

////////////////////////////////////////////////////////////////

Code_For_Ast & Assignment_Ast::compile()
{
	CHECK_INVARIANT((lhs != NULL), "Lhs cannot be null in Assignment_Ast");
	CHECK_INVARIANT((rhs != NULL), "Rhs cannot be null in Assignment_Ast");

	Code_For_Ast & load_stmt = rhs->compile();

	Register_Descriptor * load_register = load_stmt.get_reg();
	CHECK_INVARIANT(load_register, "Load register cannot be null in Assignment_Ast");
	load_register->set_use_for_expr_result();

	Code_For_Ast store_stmt = lhs->create_store_stmt(load_register);

	CHECK_INVARIANT((load_register != NULL), "Load register cannot be null in Assignment_Ast");
	load_register->reset_use_for_expr_result();

	// Store the statement in ic_list

	list<Icode_Stmt *> & ic_list = *new list<Icode_Stmt *>;

	if (load_stmt.get_icode_list().empty() == false)
		ic_list = load_stmt.get_icode_list();

	if (store_stmt.get_icode_list().empty() == false)
		ic_list.splice(ic_list.end(), store_stmt.get_icode_list());

	Code_For_Ast * assign_stmt;
	if (ic_list.empty() == false)
		assign_stmt = new Code_For_Ast(ic_list, load_register);

	return *assign_stmt;
}


/////////////////////////////////////////////////////////////////

Code_For_Ast & Name_Ast::compile()
{
	
}

Code_For_Ast & Name_Ast::create_store_stmt(Register_Descriptor * store_register)
{
	
}


///////////////////////////////////////////////////////////////////////////////

template <class DATA_TYPE>
Code_For_Ast & Number_Ast<DATA_TYPE>::compile()
{
	
}

///////////////////////////////////////////////////////////////////////////////

Code_For_Ast & Relational_Expr_Ast::compile()
{
	
}

//////////////////////////////////////////////////////////////////////

Code_For_Ast & Boolean_Expr_Ast::compile()
{
	
}
///////////////////////////////////////////////////////////////////////

Code_For_Ast & Selection_Statement_Ast::compile()
{
	
}

///////////////////////////////////////////////////////////////////////////////////////////

Code_For_Ast & Iteration_Statement_Ast::compile()
{
	
}

///////////////////////////////////////////////////////////////////////////////////////////

Code_For_Ast & Plus_Ast::compile()
{
	
}

/////////////////////////////////////////////////////////////////

Code_For_Ast & Minus_Ast::compile()
{
	
}

//////////////////////////////////////////////////////////////////

Code_For_Ast & Mult_Ast::compile()
{
	
}

////////////////////////////////////////////////////////////////////

Code_For_Ast & Conditional_Operator_Ast::compile()
{
	
}


////////////////////////////////////////////////////////////////////

Code_For_Ast & Divide_Ast::compile()
{
	
}


//////////////////////////////////////////////////////////////////////

Code_For_Ast & UMinus_Ast::compile()
{
	
}

//////////////////////////////////////////////////////////////////////////////

Code_For_Ast & Sequence_Ast::compile()
{
	
}

void Sequence_Ast::print_assembly(ostream & file_buffer)
{
	
}

void Sequence_Ast::print_icode(ostream & file_buffer)
{
	
}

//////////////////////////////////////////////////////////////////////////////

template class Number_Ast<double>;
template class Number_Ast<int>;
