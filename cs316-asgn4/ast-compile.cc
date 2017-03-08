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
	CHECK_INVARIANT((variable_symbol_entry != NULL), "variable_symbol_entry cannot be null in Name_Ast");
	Mem_Addr_Opd * variable = new Mem_Addr_Opd(*variable_symbol_entry);

	Register_Descriptor * result_register;
	Tgt_Op stmt_operator;
	machine_desc_object.clear_local_register_mappings();

	if(node_data_type == int_data_type){
		stmt_operator = load;
		result_register = machine_desc_object.get_new_register<gp_data>();
	}
	else{
		stmt_operator = load_d;
		result_register = machine_desc_object.get_new_register<float_reg>();
	}

	Register_Addr_Opd * result = new Register_Addr_Opd(result_register);
	Move_IC_Stmt * move_ic_stmt = new Move_IC_Stmt(stmt_operator, variable, result);

	list<Icode_Stmt *> & ic_list = *new list<Icode_Stmt *>;
	ic_list.push_back(move_ic_stmt);

	Code_For_Ast * assign_stmt = new Code_For_Ast(ic_list, result_register);
	return *assign_stmt;
}

Code_For_Ast & Name_Ast::create_store_stmt(Register_Descriptor * store_register)
{
	CHECK_INVARIANT((store_register != NULL), "store_register cannot be null in Name_Ast");
	Register_Addr_Opd * variable = new Register_Addr_Opd(store_register);

	Mem_Addr_Opd * variable_name = new Mem_Addr_Opd(*variable_symbol_entry);
	Tgt_Op stmt_operator;

	if (node_data_type == int_data_type){
		stmt_operator = store;
	}

	else{
		stmt_operator = store_d;
	}

	Move_IC_Stmt * move_ic_stmt = new Move_IC_Stmt(stmt_operator, variable, variable_name);

	list<Icode_Stmt *> & ic_list = *new list<Icode_Stmt *>;
	ic_list.push_back(move_ic_stmt);

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
	machine_desc_object.clear_local_register_mappings();

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

	machine_desc_object.clear_local_register_mappings();
	Register_Descriptor * result_register = machine_desc_object.get_new_register<gp_data>();
	Register_Addr_Opd * result = new Register_Addr_Opd(result_register);

	Tgt_Op stmt_operator;

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

//////////////////////////////////////////////////////////////////////

Code_For_Ast & Boolean_Expr_Ast::compile()
{
	CHECK_INVARIANT((rhs_op != NULL), "Rhs of Boolean_Expr_Ast cannot be null");
	CHECK_INVARIANT((lhs_op != NULL || ast_num_child == unary_arity), "Lhs of Boolean_Expr_Ast cannot be null");

	list<Icode_Stmt *> & ic_list = *new list<Icode_Stmt *>;
	Register_Addr_Opd * lhs_operand;
	Register_Descriptor * lhs_register;

	machine_desc_object.clear_local_register_mappings();
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
	machine_desc_object.clear_local_register_mappings();

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
	machine_desc_object.clear_local_register_mappings();

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
	machine_desc_object.clear_local_register_mappings();

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
	Tgt_Op stmt_operator = or_t;
	machine_desc_object.clear_local_register_mappings();

	if (node_data_type == int_data_type){
		result_register = machine_desc_object.get_new_register<gp_data>();
	}
	else{
		result_register = machine_desc_object.get_new_register<float_reg>();
	}

	Register_Addr_Opd * result = new Register_Addr_Opd(result_register);
	Register_Addr_Opd * lhs_operand = new Register_Addr_Opd(lhs_register);
	Register_Addr_Opd * rhs_operand = new Register_Addr_Opd(rhs_register);

	Compute_IC_Stmt* add_stmt = new Compute_IC_Stmt(stmt_operator, lhs_operand, zero_opd, result);
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
	machine_desc_object.clear_local_register_mappings();

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
	machine_desc_object.clear_local_register_mappings();

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

//////////////////////////////////////////////////////////////////////////////

template class Number_Ast<double>;
template class Number_Ast<int>;
