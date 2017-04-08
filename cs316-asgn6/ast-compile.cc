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

const string String_Ast_Type = typeid(String_Ast).name();
const string Number_Ast_Double_Type = typeid(Number_Ast<double>).name();
const string Number_Ast_Int_Type = typeid(Number_Ast<int>).name();

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
	CHECK_INVARIANT((variable_symbol_entry != NULL), "variable_symbol_entry cannot be null in Name_Ast");
	Mem_Addr_Opd * variable = new Mem_Addr_Opd(*variable_symbol_entry);

	Register_Descriptor * result_register;
	Tgt_Op stmt_operator;
	// machine_desc_object.clear_local_register_mappings();

	if(node_data_type == int_data_type){
		stmt_operator = load;
		result_register = machine_desc_object.get_new_register<gp_data>();
	}
	else{
		stmt_operator = load_d;
		result_register = machine_desc_object.get_new_register<float_reg>();
	}

	CHECK_INVARIANT((result_register != NULL), "Result register cannot be null in Name_Ast");

	Register_Addr_Opd * result = new Register_Addr_Opd(result_register);
	Move_IC_Stmt * load_stmt = new Move_IC_Stmt(stmt_operator, variable, result);

	list<Icode_Stmt *> & ic_list = *new list<Icode_Stmt *>;
	ic_list.push_back(load_stmt);

	Code_For_Ast * assign_stmt = new Code_For_Ast(ic_list, result_register);
	return *assign_stmt;
}

Code_For_Ast & Name_Ast::create_store_stmt(Register_Descriptor * store_register)
{
	CHECK_INVARIANT((store_register != NULL), "store_register cannot be null in Name_Ast");
	CHECK_INVARIANT(variable_symbol_entry, "Variable symbol entry cannot be null in Name_Ast");

	Register_Addr_Opd * variable = new Register_Addr_Opd(store_register);
	Mem_Addr_Opd * variable_name = new Mem_Addr_Opd(*variable_symbol_entry);

	Tgt_Op stmt_operator;
	// machine_desc_object.clear_local_register_mappings();
	if (node_data_type == int_data_type){
		stmt_operator = store;
	}
	else{
		stmt_operator = store_d;
	}
	Move_IC_Stmt * store_stmt = new Move_IC_Stmt(stmt_operator, variable, variable_name);

	CHECK_INVARIANT((store_stmt != NULL), "Store statement cannot be null in Name_Ast");

	list<Icode_Stmt *> & ic_list = *new list<Icode_Stmt *>;
	ic_list.push_back(store_stmt);

	Code_For_Ast * assign_stmt = new Code_For_Ast(ic_list, store_register);
	return *assign_stmt;
}


///////////////////////////////////////////////////////////////////////////////

template <class DATA_TYPE>
Code_For_Ast & Number_Ast<DATA_TYPE>::compile()
{
	//CHECK_INVARIANT((constant != NULL), "constant cannot be null in Number_Ast");
	Const_Opd<DATA_TYPE> * variable = new Const_Opd<DATA_TYPE>(constant);

	Register_Descriptor * result_register;
	Tgt_Op stmt_operator;
	// machine_desc_object.clear_local_register_mappings();

	if (node_data_type == int_data_type){
		stmt_operator = imm_load;
		result_register = machine_desc_object.get_new_register<gp_data>();
	}

	else{
		stmt_operator = imm_load_d;
		result_register = machine_desc_object.get_new_register<float_reg>();
	}

	Register_Addr_Opd * result = new Register_Addr_Opd(result_register);
	Move_IC_Stmt * add_stmt = new Move_IC_Stmt(stmt_operator, variable, result);

	list<Icode_Stmt *> & ic_list = *new list<Icode_Stmt *>;
	ic_list.push_back(add_stmt);

	Code_For_Ast * assign_stmt = new Code_For_Ast(ic_list, result_register);
	return *assign_stmt;
}

///////////////////////////////////////////////////////////////////////////////

Code_For_Ast & Relational_Expr_Ast::compile()
{
	CHECK_INVARIANT((lhs_condition != NULL), "Lhs cannot be null in Relational_Expr_Ast");
	CHECK_INVARIANT((rhs_condition != NULL), "Rhs cannot be null in Relational_Expr_Ast");

	Code_For_Ast & lhs_stmt = lhs_condition->compile();
	Register_Descriptor * lhs_register = lhs_stmt.get_reg();
	lhs_register->set_use_for_expr_result();
	CHECK_INVARIANT(lhs_register, "LHS register cannot be null in Relational_Expr_Ast");

	Code_For_Ast & rhs_stmt = rhs_condition->compile();
	Register_Descriptor * rhs_register = rhs_stmt.get_reg();
	lhs_register->set_use_for_expr_result();
	CHECK_INVARIANT(rhs_register, "RHS register cannot be null in Relational_Expr_Ast");

	list<Icode_Stmt *> & ic_list = *new list<Icode_Stmt *>;
	if (lhs_stmt.get_icode_list().empty() == false)
		ic_list = lhs_stmt.get_icode_list();

	if (rhs_stmt.get_icode_list().empty() == false)
		ic_list.splice(ic_list.end(), rhs_stmt.get_icode_list());

	// machine_desc_object.clear_local_register_mappings();
	Register_Descriptor * result_register = machine_desc_object.get_new_register<gp_data>();
	Register_Addr_Opd * result = new Register_Addr_Opd(result_register);

	Tgt_Op stmt_operator;

	Data_Type dt = lhs_condition->get_data_type();

	switch(dt)
	{
		case double_data_type:
			switch(rel_op)
			{
			case greater_than:
			case less_equalto:
				stmt_operator = sle_d;
				break;
			case greater_equalto:
			case less_than:
				stmt_operator = slt_d;
				break;
			case not_equalto:
			case equalto:
				stmt_operator = seq_d;
				break;
			}
			break;
		case int_data_type:
			switch(rel_op)
			{
			case less_equalto:
				stmt_operator = sle;
				break;
			case less_than:
				stmt_operator = slt;
				break;
			case greater_than:
				stmt_operator = sgt;
				break;
			case greater_equalto:
				stmt_operator = sge;
				break;
			case equalto:
				stmt_operator = seq;
				break;
			case not_equalto:
				stmt_operator = sne;
				break;
			}
			break;
	}


	Register_Addr_Opd * lhs_operand = new Register_Addr_Opd(lhs_register);
	Register_Addr_Opd * rhs_operand = new Register_Addr_Opd(rhs_register);
	if(dt == int_data_type)
	{
		Compute_IC_Stmt * add_stmt = new Compute_IC_Stmt(stmt_operator, lhs_operand, rhs_operand, result);
		ic_list.push_back(add_stmt);
	}
	else
	{
		ic_list.push_back(new Move_IC_Stmt(imm_load, new Const_Opd<int>(0), result));
		ic_list.push_back(new Compute_IC_Stmt(stmt_operator, lhs_operand, rhs_operand, NULL));
		string myLabel1 = "label" + to_string(labelCounter++);
		ic_list.push_back(new Label_IC_Stmt(bc1f, NULL, myLabel1));
		ic_list.push_back(new Move_IC_Stmt(imm_load, new Const_Opd<int>(1), result));
		ic_list.push_back(new Label_IC_Stmt(label, NULL, myLabel1));
		if(rel_op == greater_than || rel_op == greater_equalto || rel_op == not_equalto )
		{
			ic_list.push_back(new Compute_IC_Stmt(not_t, result, new Const_Opd<int>(1), result));
		}
	}

	Code_For_Ast * assign_stmt;
	if (ic_list.empty() == false)
		assign_stmt = new Code_For_Ast(ic_list, result_register);

	lhs_register->reset_use_for_expr_result();
	rhs_register->reset_use_for_expr_result();

	return *assign_stmt;
}

//////////////////////////////////////////////////////////////////////

Code_For_Ast & Boolean_Expr_Ast::compile()
{
	CHECK_INVARIANT((rhs_op != NULL), "Rhs of Boolean_Expr_Ast cannot be null");
	CHECK_INVARIANT((lhs_op != NULL || ast_num_child == unary_arity), "Lhs of Boolean_Expr_Ast cannot be null");

	list<Icode_Stmt *> & ic_list = *new list<Icode_Stmt *>;
	Register_Addr_Opd * lhs_operand;
	Register_Descriptor * lhs_register;

	// machine_desc_object.clear_local_register_mappings();
	if(ast_num_child != unary_arity){
		Code_For_Ast & lhs_stmt = lhs_op->compile();
		lhs_register = lhs_stmt.get_reg();
		lhs_register->set_use_for_expr_result();
		CHECK_INVARIANT(lhs_register, "LHS register cannot be null in Boolean_Expr_Ast");

		if (lhs_stmt.get_icode_list().empty() == false)
			ic_list = lhs_stmt.get_icode_list();

		lhs_operand = new Register_Addr_Opd(lhs_register);
	}
	else{
		Const_Opd<int> * variable = new Const_Opd<int>(1);

		Tgt_Op stmt_operator=imm_load;
		lhs_register = machine_desc_object.get_new_register<gp_data>();

		lhs_operand = new Register_Addr_Opd(lhs_register);
		Move_IC_Stmt * add_stmt = new Move_IC_Stmt(stmt_operator, variable, lhs_operand);
		ic_list.push_back(add_stmt);
	}

	Code_For_Ast & rhs_stmt = rhs_op->compile();
	Register_Descriptor * rhs_register = rhs_stmt.get_reg();
	rhs_register->set_use_for_expr_result();
	CHECK_INVARIANT(rhs_register, "RHS register cannot be null in Boolean_Expr_Ast");

	if (rhs_stmt.get_icode_list().empty() == false)
		ic_list.splice(ic_list.end(), rhs_stmt.get_icode_list());

	Register_Descriptor * result_register = machine_desc_object.get_new_register<gp_data>();
	Register_Addr_Opd * result = new Register_Addr_Opd(result_register);

	Tgt_Op stmt_operator;

	switch(bool_op)
	{
		case boolean_not:
			stmt_operator = not_t;
			break;
		case boolean_or:
			stmt_operator = or_t;
			break;
		case boolean_and:
			stmt_operator = and_t;
			break;
	}

	Register_Addr_Opd * rhs_operand = new Register_Addr_Opd(rhs_register);

	if(bool_op == boolean_not)
	{
		Register_Addr_Opd* temp = rhs_operand;
		rhs_operand = lhs_operand;
		lhs_operand  = temp;
	}
	Compute_IC_Stmt * add_stmt = new Compute_IC_Stmt(stmt_operator, lhs_operand, rhs_operand, result);
	ic_list.push_back(add_stmt);

	Code_For_Ast * assign_stmt;
	if (ic_list.empty() == false)
		assign_stmt = new Code_For_Ast(ic_list, result_register);

	lhs_register->reset_use_for_expr_result();
	rhs_register->reset_use_for_expr_result();

	return *assign_stmt;
}
///////////////////////////////////////////////////////////////////////

Code_For_Ast & Selection_Statement_Ast::compile()
{
	CHECK_INVARIANT((cond != NULL), "Condition of Selection_Statement_Ast cannot be null");
	CHECK_INVARIANT((then_part != NULL), "If part of Selection_Statement_Ast cannot be null");
	CHECK_INVARIANT((else_part != NULL), "else part(can be empty though) of Selection_Statement_Ast cannot be null");

	Code_For_Ast & cond_stmt = cond->compile();
	Register_Descriptor * cond_register = cond_stmt.get_reg();
	CHECK_INVARIANT(cond_register, "cond register cannot be null in Selection_Statement_Ast");

	Code_For_Ast & then_stmt = then_part->compile();
	Code_For_Ast & else_stmt = else_part->compile();

	list<Icode_Stmt *> & ic_list = *new list<Icode_Stmt *>;
	if (cond_stmt.get_icode_list().empty() == false)
		ic_list = cond_stmt.get_icode_list();

	Register_Addr_Opd * cond_result = new Register_Addr_Opd(cond_register);
	Ics_Opd * zero_opd = new Register_Addr_Opd(machine_desc_object.spim_register_table[zero]);
	Tgt_Op if_stmt_operator = beq;

	string myLabel1 = "label" + to_string(labelCounter++);
	string myLabel2 = "label" + to_string(labelCounter++);

	Control_Flow_IC_Stmt * if_stmt = new Control_Flow_IC_Stmt(if_stmt_operator, cond_result, zero_opd, myLabel1);
	ic_list.push_back(if_stmt);

	if (then_stmt.get_icode_list().empty() == false)
		ic_list.splice(ic_list.end(), then_stmt.get_icode_list());

	Control_Flow_IC_Stmt * goto_stmt = new Control_Flow_IC_Stmt(if_stmt_operator, NULL, NULL, myLabel2);
	ic_list.push_back(goto_stmt);

	Tgt_Op label_operator = label;
	Label_IC_Stmt * label1_stmt = new Label_IC_Stmt(label_operator, NULL, myLabel1);
	ic_list.push_back(label1_stmt);

	if (else_stmt.get_icode_list().empty() == false)
		ic_list.splice(ic_list.end(), else_stmt.get_icode_list());

	Label_IC_Stmt * label2_stmt = new Label_IC_Stmt(label_operator, NULL, myLabel2);
	ic_list.push_back(label2_stmt);

	cond_register->reset_use_for_expr_result();

	Code_For_Ast * assign_stmt;
	if (ic_list.empty() == false)
		assign_stmt = new Code_For_Ast(ic_list, cond_register);
	//ambiguity in the above statement

	return *assign_stmt;

}

///////////////////////////////////////////////////////////////////////////////////////////

Code_For_Ast & Iteration_Statement_Ast::compile()
{
	CHECK_INVARIANT((cond != NULL), "Condition of Iteration_Statement_Ast cannot be null");
	CHECK_INVARIANT((body != NULL), "body of Iteration_Statement_Ast cannot be null");

	Code_For_Ast & cond_stmt = cond->compile();
	Register_Descriptor * cond_register = cond_stmt.get_reg();
	CHECK_INVARIANT(cond_register, "cond register cannot be null in Iteration_Statement_Ast");


	string myLabel1 = "label" + to_string(labelCounter++);
	string myLabel2 = "label" + to_string(labelCounter++);
	Code_For_Ast & body_stmt = body->compile();

	list<Icode_Stmt *> & ic_list = *new list<Icode_Stmt *>;

	Tgt_Op while_stmt_operator = bne;
	if(!is_do_form){
		Control_Flow_IC_Stmt * goto_stmt = new Control_Flow_IC_Stmt(while_stmt_operator, NULL, NULL, myLabel2);
		ic_list.push_back(goto_stmt);
	}

	Tgt_Op label_operator = label;
	Label_IC_Stmt * label1_stmt = new Label_IC_Stmt(label_operator, NULL, myLabel1);
	ic_list.push_back(label1_stmt);

	if (body_stmt.get_icode_list().empty() == false)
		ic_list.splice(ic_list.end(), body_stmt.get_icode_list());

	Label_IC_Stmt * label2_stmt = new Label_IC_Stmt(label_operator, NULL, myLabel2);
	ic_list.push_back(label2_stmt);

	if (cond_stmt.get_icode_list().empty() == false)
		ic_list.splice(ic_list.end(), cond_stmt.get_icode_list());

	Register_Addr_Opd * cond_result = new Register_Addr_Opd(cond_register);
	Ics_Opd * zero_opd = new Register_Addr_Opd(machine_desc_object.spim_register_table[zero]);

	Control_Flow_IC_Stmt * while_stmt = new Control_Flow_IC_Stmt(while_stmt_operator, cond_result, zero_opd, myLabel1);
	ic_list.push_back(while_stmt);

	cond_register->reset_use_for_expr_result();

	Code_For_Ast * assign_stmt;
	if (ic_list.empty() == false)
		assign_stmt = new Code_For_Ast(ic_list, cond_register);
	//ambiguity in the above statement

	return *assign_stmt;
}

///////////////////////////////////////////////////////////////////////////////////////////

Code_For_Ast & Plus_Ast::compile()
{
	CHECK_INVARIANT((lhs != NULL), "Lhs cannot be null in Plus_Ast");
	CHECK_INVARIANT((rhs != NULL), "Rhs cannot be null in Plus_Ast");

	Code_For_Ast & lhs_stmt = lhs->compile();
	Register_Descriptor * lhs_register = lhs_stmt.get_reg();
	CHECK_INVARIANT(lhs_register, "LHS register cannot be null in Plus_Ast");

	Code_For_Ast & rhs_stmt = rhs->compile();
	Register_Descriptor * rhs_register = rhs_stmt.get_reg();
	CHECK_INVARIANT(rhs_register, "RHS register cannot be null in Plus_Ast");

	list<Icode_Stmt *> & ic_list = *new list<Icode_Stmt *>;
	if (lhs_stmt.get_icode_list().empty() == false)
		ic_list = lhs_stmt.get_icode_list();

	if (rhs_stmt.get_icode_list().empty() == false)
		ic_list.splice(ic_list.end(), rhs_stmt.get_icode_list());

	Register_Descriptor * result_register;
	Tgt_Op stmt_operator;
	// machine_desc_object.clear_local_register_mappings();

	if (node_data_type == int_data_type){
		stmt_operator = add;
		result_register = machine_desc_object.get_new_register<gp_data>();
	}

	else{
		stmt_operator = add_d;
		result_register = machine_desc_object.get_new_register<float_reg>();
	}

	Register_Addr_Opd * result = new Register_Addr_Opd(result_register);
	Register_Addr_Opd * lhs_operand = new Register_Addr_Opd(lhs_register);
	Register_Addr_Opd * rhs_operand = new Register_Addr_Opd(rhs_register);

	Compute_IC_Stmt * add_stmt = new Compute_IC_Stmt(stmt_operator, lhs_operand, rhs_operand, result);
	ic_list.push_back(add_stmt);

	Code_For_Ast * assign_stmt;
	if (ic_list.empty() == false)
		assign_stmt = new Code_For_Ast(ic_list, result_register);

	lhs_register->reset_use_for_expr_result();
	rhs_register->reset_use_for_expr_result();

	return *assign_stmt;
}

/////////////////////////////////////////////////////////////////

Code_For_Ast & Minus_Ast::compile()
{
	CHECK_INVARIANT((lhs != NULL), "Lhs cannot be null in Minus_Ast");
	CHECK_INVARIANT((rhs != NULL), "Rhs cannot be null in Minus_Ast");

	Code_For_Ast & lhs_stmt = lhs->compile();
	Register_Descriptor * lhs_register = lhs_stmt.get_reg();
	CHECK_INVARIANT(lhs_register, "LHS register cannot be null in Minus_Ast");

	Code_For_Ast & rhs_stmt = rhs->compile();
	Register_Descriptor * rhs_register = rhs_stmt.get_reg();
	CHECK_INVARIANT(rhs_register, "RHS register cannot be null in Minus_Ast");

	list<Icode_Stmt *> & ic_list = *new list<Icode_Stmt *>;
	if (lhs_stmt.get_icode_list().empty() == false)
		ic_list = lhs_stmt.get_icode_list();

	if (rhs_stmt.get_icode_list().empty() == false)
		ic_list.splice(ic_list.end(), rhs_stmt.get_icode_list());

	Register_Descriptor * result_register;
	Tgt_Op stmt_operator;
	// machine_desc_object.clear_local_register_mappings();

	if (node_data_type == int_data_type){
		stmt_operator = sub;
		result_register = machine_desc_object.get_new_register<gp_data>();
	}

	else{
		stmt_operator = sub_d;
		result_register = machine_desc_object.get_new_register<float_reg>();
	}

	Register_Addr_Opd * result = new Register_Addr_Opd(result_register);
	Register_Addr_Opd * lhs_operand = new Register_Addr_Opd(lhs_register);
	Register_Addr_Opd * rhs_operand = new Register_Addr_Opd(rhs_register);

	Compute_IC_Stmt * add_stmt = new Compute_IC_Stmt(stmt_operator, lhs_operand, rhs_operand, result);
	ic_list.push_back(add_stmt);

	Code_For_Ast * assign_stmt;
	if (ic_list.empty() == false)
		assign_stmt = new Code_For_Ast(ic_list, result_register);

	lhs_register->reset_use_for_expr_result();
	rhs_register->reset_use_for_expr_result();

	return *assign_stmt;
}

//////////////////////////////////////////////////////////////////

Code_For_Ast & Mult_Ast::compile()
{
	CHECK_INVARIANT((lhs != NULL), "Lhs cannot be null in Mult_Ast");
	CHECK_INVARIANT((rhs != NULL), "Rhs cannot be null in Mult_Ast");

	Code_For_Ast & lhs_stmt = lhs->compile();
	Register_Descriptor * lhs_register = lhs_stmt.get_reg();
	CHECK_INVARIANT(lhs_register, "LHS register cannot be null in Mult_Ast");
	Code_For_Ast & rhs_stmt = rhs->compile();

	Register_Descriptor * rhs_register = rhs_stmt.get_reg();
	CHECK_INVARIANT(rhs_register, "RHS register cannot be null in Mult_Ast");


	list<Icode_Stmt *> & ic_list = *new list<Icode_Stmt *>;
	if (lhs_stmt.get_icode_list().empty() == false)
		ic_list = lhs_stmt.get_icode_list();

	if (rhs_stmt.get_icode_list().empty() == false)
		ic_list.splice(ic_list.end(), rhs_stmt.get_icode_list());

	Register_Descriptor * result_register;
	Tgt_Op stmt_operator;
	// machine_desc_object.clear_local_register_mappings();

	if (node_data_type == int_data_type){
		stmt_operator = mult;
		result_register = machine_desc_object.get_new_register<gp_data>();
	}

	else{
		stmt_operator = mult_d;
		result_register = machine_desc_object.get_new_register<float_reg>();
	}

	Register_Addr_Opd * result = new Register_Addr_Opd(result_register);
	Register_Addr_Opd * lhs_operand = new Register_Addr_Opd(lhs_register);
	Register_Addr_Opd * rhs_operand = new Register_Addr_Opd(rhs_register);

	Compute_IC_Stmt * add_stmt = new Compute_IC_Stmt(stmt_operator, lhs_operand, rhs_operand, result);
	ic_list.push_back(add_stmt);

	Code_For_Ast * assign_stmt;
	if (ic_list.empty() == false)
		assign_stmt = new Code_For_Ast(ic_list, result_register);

	lhs_register->reset_use_for_expr_result();
	rhs_register->reset_use_for_expr_result();

	return *assign_stmt;
}

////////////////////////////////////////////////////////////////////

Code_For_Ast & Conditional_Operator_Ast::compile()
{
	CHECK_INVARIANT((lhs != NULL), "Lhs cannot be null in Conditional_Operator_Ast");
	CHECK_INVARIANT((rhs != NULL), "Rhs cannot be null in Conditional_Operator_Ast");
	CHECK_INVARIANT((cond != NULL), "Condition of Conditional_Operator_Ast cannot be null");

	Code_For_Ast & cond_stmt = cond->compile();
	Register_Descriptor * cond_register = cond_stmt.get_reg();
	cond_register->set_use_for_expr_result();
	CHECK_INVARIANT(cond_register, "cond register cannot be null in Iteration_Statement_Ast");

	Code_For_Ast & lhs_stmt = lhs->compile();
	Register_Descriptor * lhs_register = lhs_stmt.get_reg();
	lhs_register->set_use_for_expr_result();
	CHECK_INVARIANT(lhs_register, "LHS register cannot be null in Mult_Ast");

	Code_For_Ast & rhs_stmt = rhs->compile();
	Register_Descriptor * rhs_register = rhs_stmt.get_reg();
	rhs_register->set_use_for_expr_result();
	CHECK_INVARIANT(rhs_register, "RHS register cannot be null in Mult_Ast");

	list<Icode_Stmt *> & ic_list = *new list<Icode_Stmt *>;

	if (cond_stmt.get_icode_list().empty() == false)
		ic_list = cond_stmt.get_icode_list();

	Register_Addr_Opd * cond_result = new Register_Addr_Opd(cond_register);
	Ics_Opd * zero_opd = new Register_Addr_Opd(machine_desc_object.spim_register_table[zero]);
	Tgt_Op if_stmt_operator = beq;

	string myLabel1 = "label" + to_string(labelCounter++);
	string myLabel2 = "label" + to_string(labelCounter++);

	Control_Flow_IC_Stmt * if_stmt = new Control_Flow_IC_Stmt(if_stmt_operator, cond_result, zero_opd, myLabel1);
	ic_list.push_back(if_stmt);

	if (lhs_stmt.get_icode_list().empty() == false)
		ic_list.splice(ic_list.end(), lhs_stmt.get_icode_list());

	Register_Descriptor * result_register;
	Tgt_Op stmt_operator;// = or_t;
	// machine_desc_object.clear_local_register_mappings();

	if (node_data_type == int_data_type){
		result_register = machine_desc_object.get_new_register<gp_data>();
		stmt_operator = mov;
	}
	else{
		result_register = machine_desc_object.get_new_register<float_reg>();
		stmt_operator = move_d;
	}

	Register_Addr_Opd * result = new Register_Addr_Opd(result_register);
	Register_Addr_Opd * lhs_operand = new Register_Addr_Opd(lhs_register);
	Register_Addr_Opd * rhs_operand = new Register_Addr_Opd(rhs_register);

	// Compute_IC_Stmt* add_stmt = new Compute_IC_Stmt(stmt_operator, lhs_operand, zero_opd, result);
	Move_IC_Stmt* add_stmt = new Move_IC_Stmt(stmt_operator, lhs_operand, result);
	ic_list.push_back(add_stmt);

	Control_Flow_IC_Stmt * goto_stmt = new Control_Flow_IC_Stmt(if_stmt_operator, NULL, NULL, myLabel2);
	ic_list.push_back(goto_stmt);

	Tgt_Op label_operator = label;
	Label_IC_Stmt * label1_stmt = new Label_IC_Stmt(label_operator, NULL, myLabel1);
	ic_list.push_back(label1_stmt);

	if (rhs_stmt.get_icode_list().empty() == false)
		ic_list.splice(ic_list.end(), rhs_stmt.get_icode_list());

	Compute_IC_Stmt* add_another_stmt = new Compute_IC_Stmt(stmt_operator, rhs_operand, zero_opd, result);
	ic_list.push_back(add_another_stmt);

	Label_IC_Stmt * label2_stmt = new Label_IC_Stmt(label_operator, NULL, myLabel2);
	ic_list.push_back(label2_stmt);

	Code_For_Ast * assign_stmt;
	if (ic_list.empty() == false)
		assign_stmt = new Code_For_Ast(ic_list, result_register);

	lhs_register->reset_use_for_expr_result();
	rhs_register->reset_use_for_expr_result();
	cond_register->reset_use_for_expr_result();

	return *assign_stmt;

}


////////////////////////////////////////////////////////////////////

Code_For_Ast & Divide_Ast::compile()
{
	CHECK_INVARIANT((lhs != NULL), "Lhs cannot be null in Divide_Ast");
	CHECK_INVARIANT((rhs != NULL), "Rhs cannot be null in Divide_Ast");

	Code_For_Ast & lhs_stmt = lhs->compile();
	Register_Descriptor * lhs_register = lhs_stmt.get_reg();
	CHECK_INVARIANT(lhs_register, "LHS register cannot be null in Divide_Ast");

	Code_For_Ast & rhs_stmt = rhs->compile();
	Register_Descriptor * rhs_register = rhs_stmt.get_reg();
	CHECK_INVARIANT(rhs_register, "RHS register cannot be null in Divide_Ast");

	list<Icode_Stmt *> & ic_list = *new list<Icode_Stmt *>;
	if (lhs_stmt.get_icode_list().empty() == false)
		ic_list = lhs_stmt.get_icode_list();

	if (rhs_stmt.get_icode_list().empty() == false)
		ic_list.splice(ic_list.end(), rhs_stmt.get_icode_list());

	Register_Descriptor * result_register;
	Tgt_Op stmt_operator;
	// machine_desc_object.clear_local_register_mappings();

	if (node_data_type == int_data_type){
		stmt_operator = divd;
		result_register = machine_desc_object.get_new_register<gp_data>();
	}

	else{
		stmt_operator = div_d;
		result_register = machine_desc_object.get_new_register<float_reg>();
	}

	Register_Addr_Opd * result = new Register_Addr_Opd(result_register);
	Register_Addr_Opd * lhs_operand = new Register_Addr_Opd(lhs_register);
	Register_Addr_Opd * rhs_operand = new Register_Addr_Opd(rhs_register);

	Compute_IC_Stmt* add_stmt = new Compute_IC_Stmt(stmt_operator, lhs_operand, rhs_operand, result);
	ic_list.push_back(add_stmt);

	Code_For_Ast * assign_stmt;
	if (ic_list.empty() == false)
		assign_stmt = new Code_For_Ast(ic_list, result_register);

	lhs_register->reset_use_for_expr_result();
	rhs_register->reset_use_for_expr_result();

	return *assign_stmt;
}


//////////////////////////////////////////////////////////////////////

Code_For_Ast & UMinus_Ast::compile()
{
	CHECK_INVARIANT((lhs != NULL), "Lhs cannot be null in UMinus_Ast");

	Code_For_Ast & lhs_stmt = lhs->compile();
	Register_Descriptor * lhs_register = lhs_stmt.get_reg();
	CHECK_INVARIANT(lhs_register, "LHS register cannot be null in UMinus_Ast");

	list<Icode_Stmt *> & ic_list = *new list<Icode_Stmt *>;
	if (lhs_stmt.get_icode_list().empty() == false)
		ic_list = lhs_stmt.get_icode_list();

	Register_Descriptor * result_register;
	Tgt_Op stmt_operator;
	// machine_desc_object.clear_local_register_mappings();

	if (node_data_type == int_data_type){
		stmt_operator = uminus;
		result_register = machine_desc_object.get_new_register<gp_data>();
	}

	else{
		stmt_operator = uminus_d;
		result_register = machine_desc_object.get_new_register<float_reg>();
	}

	Register_Addr_Opd * result = new Register_Addr_Opd(result_register);
	Register_Addr_Opd * lhs_operand = new Register_Addr_Opd(lhs_register);

	Compute_IC_Stmt* add_stmt = new Compute_IC_Stmt(stmt_operator, lhs_operand, NULL, result);
	ic_list.push_back(add_stmt);

	Code_For_Ast * assign_stmt;
	if (ic_list.empty() == false)
		assign_stmt = new Code_For_Ast(ic_list, result_register);

	lhs_register->reset_use_for_expr_result();

	return *assign_stmt;
}

//////////////////////////////////////////////////////////////////////////////

Code_For_Ast & Sequence_Ast::compile()
{
	for (auto it=statement_list.begin(); it != statement_list.end(); ++it)
	{
		Code_For_Ast & codeForAst = (*it)->compile();
		if (codeForAst.get_icode_list().empty() == false)
			sa_icode_list.splice(sa_icode_list.end(),codeForAst.get_icode_list());
	}

	Code_For_Ast * assign_stmt;
	assign_stmt = new Code_For_Ast(sa_icode_list, NULL);
	return *assign_stmt;
}

void Sequence_Ast::print_assembly(ostream & file_buffer)
{
	for (auto it=sa_icode_list.begin(); it != sa_icode_list.end(); ++it)
	{
		(*it)->print_assembly(file_buffer);
	}
}

void Sequence_Ast::print_icode(ostream & file_buffer)
{
	for (auto it=sa_icode_list.begin(); it != sa_icode_list.end(); ++it)
	{
		(*it)->print_icode(file_buffer);
	}
}


Call_Ast::Call_Ast(string fn_name, int line)
{
	this->procedure = program_object.get_procedure(fn_name);
	this->lineno = line;
	this->node_data_type = procedure->get_return_type();
	if(node_data_type == int_data_type)
		set_register(machine_desc_object.spim_register_table[v1]);
	else if (node_data_type == double_data_type)
		set_register(machine_desc_object.spim_register_table[f0]);
}

Code_For_Ast & Call_Ast::compile()
{

	list<Icode_Stmt*> sa_icode_list;

	Register_Addr_Opd * sp_reg = new Register_Addr_Opd(machine_desc_object.spim_register_table[sp]);
	Const_Opd<int> * rhs_operand;
	int sp_offset = procedure->get_formal_symbol_table_size();

	if(sp_offset != 0){
		rhs_operand = new Const_Opd<int>(sp_offset);
		Compute_IC_Stmt * sub_stmt =  new Compute_IC_Stmt(sub, sp_reg , rhs_operand, sp_reg);
		if(sub_stmt != NULL)
			sa_icode_list.push_back(sub_stmt);
	}
	Tgt_Op stmt_operator;
	list<Symbol_Table_Entry*> s = procedure->get_symbol_entries();
	auto it2 = s.begin();
	for(auto it = argument_list.rbegin(); it != argument_list.rend(), it2 != s.end(); ++it, it2++)
	{

		Data_Type node_data_type = (*it)->get_data_type();
		Code_For_Ast & load_stmt = (*it)->compile();

		Register_Descriptor * load_register = load_stmt.get_reg();
		CHECK_INVARIANT(load_register, "Load register cannot be null in Call_Ast");

		Register_Addr_Opd * variable = new Register_Addr_Opd(load_register);
		Mem_Addr_Opd * variable_name = new Mem_Addr_Opd(*(*it2));
		if (node_data_type == int_data_type){
			stmt_operator = store;
		}
		else{
			stmt_operator = store_d;
		}
		Move_IC_Stmt * store_stmt = new Move_IC_Stmt(stmt_operator, variable, variable_name);
		CHECK_INVARIANT((store_stmt != NULL), "Store statement cannot be null");

		CHECK_INVARIANT((load_register != NULL), "Load register cannot be null in Call_Ast");
		load_register->reset_use_for_expr_result();

		list<Icode_Stmt *> & ic_list = *new list<Icode_Stmt *>;

		if (load_stmt.get_icode_list().empty() == false)
			ic_list = load_stmt.get_icode_list();

		ic_list.push_back(store_stmt);

		if (ic_list.empty() == false)
			sa_icode_list.splice(sa_icode_list.end(), ic_list);
	}
	sa_icode_list.push_back(new Call_IC_stmt(procedure->get_proc_name()));

	if(sp_offset != 0)
		sa_icode_list.push_back(new Compute_IC_Stmt(add, sp_reg, rhs_operand , sp_reg));

	Register_Descriptor * result_register;
	if(node_data_type == int_data_type){
		stmt_operator = mov;
		result_register = machine_desc_object.get_new_register<gp_data>();
	}
	else if(node_data_type == double_data_type ){
		stmt_operator = move_d;
		result_register = machine_desc_object.get_new_register<float_reg>();
	}
	if(node_data_type != void_data_type)
		sa_icode_list.push_back(new Move_IC_Stmt(stmt_operator, new Register_Addr_Opd(result_reg),
		 new Register_Addr_Opd(result_register)));

	machine_desc_object.clear_local_register_mappings();
	Code_For_Ast * assign_stmt;
	assign_stmt = new Code_For_Ast(sa_icode_list, result_register);
	return *assign_stmt;
};

void Call_Ast::print(ostream & file_buffer)
{
	return;
}

Code_For_Ast & Return_Ast::compile()
{
	list<Icode_Stmt*> sa_icode_list;
	Register_Descriptor * return_reg = NULL;
	if(node_data_type != void_data_type){
		if(node_data_type == int_data_type)
			return_reg = machine_desc_object.spim_register_table[v1];
		else if (node_data_type == double_data_type)
			return_reg = machine_desc_object.spim_register_table[f0];

		Code_For_Ast & load_stmt = ret_val->compile();

		Register_Descriptor * load_register = load_stmt.get_reg();
		CHECK_INVARIANT(load_register, "Load register cannot be null in Call_Ast");
		load_register->set_use_for_expr_result();

		Register_Addr_Opd * variable = new Register_Addr_Opd(load_register);
		Register_Addr_Opd * variable_name = new Register_Addr_Opd(return_reg);
		Tgt_Op stmt_operator;
		if (node_data_type == int_data_type){
			stmt_operator = mov;
		}
		else{
			stmt_operator = move_d;
		}
		Move_IC_Stmt * store_stmt = new Move_IC_Stmt(stmt_operator, variable, variable_name);
		CHECK_INVARIANT((store_stmt != NULL), "Store statement cannot be null");

		CHECK_INVARIANT((load_register != NULL), "Load register cannot be null in Call_Ast");
		load_register->reset_use_for_expr_result();

		if (load_stmt.get_icode_list().empty() == false)
			sa_icode_list = load_stmt.get_icode_list();

		sa_icode_list.push_back(store_stmt);
	}
	sa_icode_list.push_back(new Label_IC_Stmt(j, NULL, "epilogue_" + proc->get_proc_name()));

	Code_For_Ast * assign_stmt;
	assign_stmt = new Code_For_Ast(sa_icode_list, return_reg);
	return *assign_stmt;
}

void Return_Ast::print(ostream & file_buffer)
{
	return;
}

Code_For_Ast & Print_Ast::compile()
{
	list <Icode_Stmt*> sa_icode_list;
	Code_For_Ast & load_stmt = ast_to_print->compile();
	if (load_stmt.get_icode_list().empty() == false)
		sa_icode_list.splice(sa_icode_list.end(), load_stmt.get_icode_list());
	Register_Addr_Opd * sp_reg = new Register_Addr_Opd(machine_desc_object.spim_register_table[sp]);
	Const_Opd<int> * rhs_operand = new Const_Opd<int>(-4);
	string name = "sp";
	Symbol_Table_Entry * symbol_entry = new Symbol_Table_Entry(name, int_data_type, 0, sp_ref);
	symbol_entry->set_symbol_scope(local);
	Mem_Addr_Opd * stack_opd = new Mem_Addr_Opd(*symbol_entry);
	Register_Addr_Opd* reg_v0 = new Register_Addr_Opd(machine_desc_object.spim_register_table[v0]);
	Register_Addr_Opd* reg_a0 = new Register_Addr_Opd(machine_desc_object.spim_register_table[a0]);
	Register_Addr_Opd* reg_f12 = new Register_Addr_Opd(machine_desc_object.spim_register_table[f12]);

	Compute_IC_Stmt * sub_stmt =  new Compute_IC_Stmt(imm_add, sp_reg , rhs_operand, sp_reg);
	sa_icode_list.push_back(sub_stmt);
	sa_icode_list.push_back(new Move_IC_Stmt(store, reg_v0, stack_opd));
	sa_icode_list.push_back(sub_stmt);
	sa_icode_list.push_back(new Move_IC_Stmt(store, reg_a0, stack_opd));
	sa_icode_list.push_back( new Compute_IC_Stmt(imm_add, sp_reg ,  new Const_Opd<int>(-8), sp_reg));
	sa_icode_list.push_back(new Move_IC_Stmt(store_d, reg_f12,  stack_opd));

	Register_Descriptor * load_register = load_stmt.get_reg();
	string type_name = typeid(*ast_to_print).name();
	Const_Opd<int> * arg;
	if (type_name == String_Ast_Type)
	{
		arg = new Const_Opd<int>(4);
		sa_icode_list.push_back(new Move_IC_Stmt(la, new String_Addr_Opd(
			((String_Ast*)ast_to_print)->get_label()), reg_a0));
	}
	else if(ast_to_print->get_data_type() == int_data_type || (type_name == Number_Ast_Int_Type))
	{
		arg = new Const_Opd<int>(1);
		sa_icode_list.push_back(new Move_IC_Stmt(mov, new Register_Addr_Opd(load_register), reg_a0));
	}
	else if(ast_to_print->get_data_type() == double_data_type || (type_name == Number_Ast_Double_Type))
	{
		arg = new Const_Opd<int>(3);
		sa_icode_list.push_back(new Move_IC_Stmt(move_d, new Register_Addr_Opd(load_register), reg_f12));
	}
	sa_icode_list.push_back(new Move_IC_Stmt(imm_load, arg, reg_v0));
	sa_icode_list.push_back(new Label_IC_Stmt(syscall, NULL, ""));


	sa_icode_list.push_back(new Move_IC_Stmt(load_d,  stack_opd, reg_f12));
	sa_icode_list.push_back(new Compute_IC_Stmt(imm_add, sp_reg ,  new Const_Opd<int>(8), sp_reg));
	sa_icode_list.push_back(new Move_IC_Stmt(load, stack_opd, reg_a0));
	Compute_IC_Stmt* add_stmt = new Compute_IC_Stmt(imm_add, sp_reg ,  new Const_Opd<int>(4), sp_reg);
	sa_icode_list.push_back(add_stmt);
	sa_icode_list.push_back(new Move_IC_Stmt(load, stack_opd, reg_v0));
	sa_icode_list.push_back(add_stmt);
	if(load_register != NULL)
		load_register->reset_use_for_expr_result();
	Code_For_Ast * assign_stmt;
	assign_stmt = new Code_For_Ast(sa_icode_list, NULL);
	return *assign_stmt;
}

void Print_Ast::print(ostream & file_buffer)
{

}

Code_For_Ast & String_Ast::compile()
{

	list<Icode_Stmt *> ic_list;
	Code_For_Ast * assign_stmt = new Code_For_Ast(ic_list, NULL);
	return *assign_stmt;
}

//////////////////////////////////////////////////////////////////////////////

template class Number_Ast<double>;
template class Number_Ast<int>;
