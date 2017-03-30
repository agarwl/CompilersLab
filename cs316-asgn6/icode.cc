#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <map>

using namespace std;

#include "common-classes.hh"
#include "error-display.hh"
#include "icode.hh"
#include "reg-alloc.hh"
#include "symbol-table.hh"
#include "ast.hh"
#include "program.hh"

/****************************** Class Ics_Opd *****************************/

Register_Descriptor * Ics_Opd::get_reg()
{
	//TODO
	stringstream msg;
	msg << "No get_reg() function for " << typeid(*this).name();
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, msg.str());
}

/****************************** Class Mem_Addr_Opd *****************************/

Mem_Addr_Opd::Mem_Addr_Opd(Symbol_Table_Entry & se)
{
	//TODO
	this->symbol_entry = &se;
}

Mem_Addr_Opd & Mem_Addr_Opd::operator=(const Mem_Addr_Opd & rhs)
{
	//TODO
	*this = rhs;
	return *this;
}

void Mem_Addr_Opd::print_ics_opd(ostream & file_buffer)
{
	//TODO
	file_buffer << symbol_entry->get_variable_name();
}

void Mem_Addr_Opd::print_asm_opd(ostream & file_buffer)
{
	Table_Scope symbol_scope = symbol_entry->get_symbol_scope();

	CHECK_INVARIANT(((symbol_scope == local) || (symbol_scope == global)),
			"Wrong scope value");

	if (symbol_scope == local)
	{
		int offset = symbol_entry->get_start_offset();
		file_buffer << offset << "($fp)";
	}
	else
		file_buffer << symbol_entry->get_variable_name();
}

/****************************** Class Register_Addr_Opd *****************************/

Register_Addr_Opd::Register_Addr_Opd(Register_Descriptor * reg)
{
	//TODO
	this->register_description = reg;
}

Register_Descriptor * Register_Addr_Opd::get_reg()
{
	//TODO
	return this->register_description;
}

Register_Addr_Opd& Register_Addr_Opd::operator=(const Register_Addr_Opd& rhs)
{
	//TODO
	*this = rhs;
	return *this;
}

void Register_Addr_Opd::print_ics_opd(ostream & file_buffer)
{
	//TODO
	file_buffer << register_description->get_name();
}

void Register_Addr_Opd::print_asm_opd(ostream & file_buffer)
{
	//TODO
	string reg_name;
	Spim_Register reg = register_description->get_register();
	switch(reg)
	{
		case none:
			reg_name = "none";break;/* dummy to indicate no register */
		case zero:
			reg_name = "zero";break;/* constant register */
		case v0:
			reg_name = "v0";break;/* expression result register */
		case v1:
			reg_name = "v1";break;/* function result register */
		case a0:
			reg_name = "a0";break;/* argument register */
		case a1:
			reg_name = "a1";break;
		case a2:
			reg_name = "a2";break;
		case a3:
			reg_name = "a3";break;
		case t0:
		    reg_name = "t0";break;  /* temporary caller-save registers */
		case t1:
			reg_name = "t1";break;
		case t2:
			reg_name = "t2";break;
		case t3:
		 	reg_name = "t3";break;
		case t4:
		 	reg_name = "t4";break;
		case t5:
		 	reg_name = "t5";break;
		case t6:
		 	reg_name = "t6";break;
		case t7:
		 	reg_name = "t7";break;
		case t8:
		 	reg_name = "t8";break;
		case t9:
		 	reg_name = "t9";break;
		case s0:
			reg_name = "s0";break;/* temporary callee-save registers */
		case s1:
			reg_name = "s1";break;
		case s2:
			reg_name = "s2";break;
		case s3:
			reg_name = "s3";break;
		case s4:
			reg_name = "s4";break;
		case s5:
			reg_name = "s5";break;
		case s6:
			reg_name = "s6";break;
		case s7:
			reg_name = "s7";break;
		case mfc:
			reg_name = "mfc";break;/* float register to int register */
		case mtc:
			reg_name = "mtc";break;/* int register to float register */
		case f0:
		 	reg_name = "f0";break;/* floating point registers */
		case f2:
			reg_name = "f2";break;
		case f4:
			reg_name = "f4";break;
		case f6:
			reg_name = "f6";break;
		case f8:
			reg_name = "f8";break;
		case f10:
			reg_name = "f10";break;
		case f12:
			reg_name = "f12";break;
		case f14:
			reg_name = "f14";break;
		case f16:
			reg_name = "f16";break;
		case f18:
			reg_name = "f18";break;
		case f20:
			reg_name = "f20";break;
		case f22:
			reg_name = "f22";break;
		case f24:
			reg_name = "f24";break;
		case f26:
			reg_name = "f26";break;
		case f28:
			reg_name = "f28";break;
		case f30:
			reg_name = "f30";break;
		case gp:
			reg_name = "gp";break;	/* global data pointer register */
		case sp:
			reg_name = "sp";break;	/* stack pointer register */
		case fp:
			reg_name = "fp";break;	/* frame pointer register */
		case ra:
			reg_name = "ra";break;	/* return address register */
		default : break;
	}
	file_buffer << "$" << reg_name;
}

/****************************** Class Const_Opd *****************************/

template <class DATA_TYPE>
Const_Opd<DATA_TYPE>::Const_Opd(DATA_TYPE n)
{
	//TODO
	this->num = n;
}

template <class DATA_TYPE>
Const_Opd<DATA_TYPE> & Const_Opd<DATA_TYPE>::operator=(const Const_Opd<DATA_TYPE> & rhs)
{
	//TODO
	// this->num = rhs.num;
	*this = rhs;
	return *this;
}

template <class DATA_TYPE>
void Const_Opd<DATA_TYPE>::print_ics_opd(ostream & file_buffer)
{
	//TODO
	file_buffer << num;
}

template <class DATA_TYPE>
void Const_Opd<DATA_TYPE>::print_asm_opd(ostream & file_buffer)
{
	//TODO
	file_buffer << num;
}

/****************************** Class Icode_Stmt *****************************/

Instruction_Descriptor & Icode_Stmt::get_op()
{
	return op_desc;
}

Ics_Opd * Icode_Stmt::get_opd1()
{
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "virtual method get_Opd1 not implemented");
}

Ics_Opd * Icode_Stmt::get_opd2()
{
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "virtual method get_Opd2 not implemented");
}

Ics_Opd * Icode_Stmt::get_result()
{
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "virtual method get_Result not implemented");
}

void Icode_Stmt::set_opd1(Ics_Opd * ics_opd)
{
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "virtual method set_Opd1 not implemented");
}

void Icode_Stmt::set_opd2(Ics_Opd * ics_opd)
{
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "virtual method set_Opd2 not implemented");
}

void Icode_Stmt::set_result(Ics_Opd * ics_opd)
{
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "virtual methos set_Result not implemented");
}

/*************************** Class Move_IC_Stmt *****************************/

Move_IC_Stmt::Move_IC_Stmt(Tgt_Op op, Ics_Opd * o1, Ics_Opd * res)
{
	CHECK_INVARIANT((machine_desc_object.spim_instruction_table[op] != NULL),
			"Instruction description in spim table cannot be null");

	op_desc = *(machine_desc_object.spim_instruction_table[op]);
	opd1 = o1;
	result = res;
}

Ics_Opd * Move_IC_Stmt::get_opd1()          { return opd1; }
Ics_Opd * Move_IC_Stmt::get_result()        { return result; }

void Move_IC_Stmt::set_opd1(Ics_Opd * io)   { opd1 = io; }
void Move_IC_Stmt::set_result(Ics_Opd * io) { result = io; }

Move_IC_Stmt& Move_IC_Stmt::operator=(const Move_IC_Stmt& rhs)
{
	*this = rhs;
	return *this;
}

void Move_IC_Stmt::print_icode(ostream & file_buffer)
{
	CHECK_INVARIANT (opd1, "Opd1 cannot be NULL for a move IC Stmt");
	CHECK_INVARIANT (result, "Result cannot be NULL for a move IC Stmt");

	string operation_name = op_desc.get_name();

	Icode_Format ic_format = op_desc.get_ic_format();

	switch (ic_format)
	{
	case i_r_op_o1:
			file_buffer << "\t" << operation_name << ":    \t";
			result->print_ics_opd(file_buffer);
			file_buffer << " <- ";
			opd1->print_ics_opd(file_buffer);
			file_buffer << "\n";

			break;

	default: CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH,
				"Intermediate code format not supported");
		break;
	}
}

void Move_IC_Stmt::print_assembly(ostream & file_buffer)
{
	CHECK_INVARIANT (opd1, "Opd1 cannot be NULL for a move IC Stmt");
	CHECK_INVARIANT (result, "Result cannot be NULL for a move IC Stmt");
	string op_name = op_desc.get_mnemonic();
	Assembly_Format assem_format = op_desc.get_assembly_format();

	switch (assem_format)
	{
	case a_op_r_o1:
			file_buffer << "\t" << op_name << " ";
			result->print_asm_opd(file_buffer);
			file_buffer << ", ";
			opd1->print_asm_opd(file_buffer);
			file_buffer << "\n";

			break;

	case a_op_o1_r:
			file_buffer << "\t" << op_name << " ";
			opd1->print_asm_opd(file_buffer);
			file_buffer << ", ";
			result->print_asm_opd(file_buffer);
			file_buffer << "\n";

			break;

	default: CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Intermediate code format not supported");
		break;
	}
}
/*******************************************************************************/

Compute_IC_Stmt::Compute_IC_Stmt(Tgt_Op op, Ics_Opd * o1, Ics_Opd * o2, Ics_Opd * res)
{
	CHECK_INVARIANT((machine_desc_object.spim_instruction_table[op] != NULL),
			"Instruction description in spim table cannot be null");

	op_desc = *(machine_desc_object.spim_instruction_table[op]);
	opd1 = o1;
	opd2 = o2;
	result = res;
}

Ics_Opd * Compute_IC_Stmt::get_opd1()          { return opd1; }
Ics_Opd * Compute_IC_Stmt::get_opd2()          { return opd2; }
Ics_Opd * Compute_IC_Stmt::get_result()        { return result; }

void Compute_IC_Stmt::set_opd1(Ics_Opd * io)   { opd1 = io; }
void Compute_IC_Stmt::set_opd2(Ics_Opd * io)   { opd2 = io; }
void Compute_IC_Stmt::set_result(Ics_Opd * io) { result = io; }

Compute_IC_Stmt& Compute_IC_Stmt::operator=(const Compute_IC_Stmt& rhs)
{
	*this = rhs;
	return *this;
}

void Compute_IC_Stmt::print_icode(ostream & file_buffer)
{
	CHECK_INVARIANT (result, "Result cannot be NULL for a compute IC Stmt");

	string operation_name = op_desc.get_name();

	Icode_Format ic_format = op_desc.get_ic_format();

	switch (ic_format)
	{
		case i_r_o1_op_o2:
				file_buffer << "\t" << operation_name << ":    \t";
				result->print_ics_opd(file_buffer);
				file_buffer << " <- ";
				opd1->print_ics_opd(file_buffer);
				file_buffer << " , ";
				opd2->print_ics_opd(file_buffer);
				file_buffer << "\n";
				break;
		case i_op_o1_r:
		case i_op_r_o1:
		case i_r_op_o1:
				file_buffer << "\t" << operation_name << ":    \t";
				result->print_ics_opd(file_buffer);
				file_buffer << " <- ";
				opd1->print_ics_opd(file_buffer);
				file_buffer << "\n";
				break;


	default: CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH,
				"Intermediate code format not supported");
		break;
	}
}

void Compute_IC_Stmt::print_assembly(ostream & file_buffer)
{
	CHECK_INVARIANT (result, "Result cannot be NULL for a compute IC Stmt");

	string op_name = op_desc.get_mnemonic();
	Assembly_Format assem_format = op_desc.get_assembly_format();

	switch (assem_format)
	{
		case a_op_r_o1_o2:
				file_buffer << "\t" << op_name << " ";
				result->print_asm_opd(file_buffer);
				file_buffer << ", ";
				opd1->print_asm_opd(file_buffer);
				file_buffer << ", ";
				opd2->print_asm_opd(file_buffer);
				file_buffer << "\n";
				break;

		case a_op_o1_o2_r:
				file_buffer << "\t" << op_name << " ";
				opd1->print_asm_opd(file_buffer);
				file_buffer << ", ";
				opd2->print_asm_opd(file_buffer);
				file_buffer << ", ";
				result->print_asm_opd(file_buffer);
				file_buffer << "\n";
				break;

		case a_op_r_o1:
				file_buffer << "\t" << op_name << " ";
				result->print_asm_opd(file_buffer);
				file_buffer << ", ";
				opd1->print_asm_opd(file_buffer);
				file_buffer << "\n";
				break;

		case a_op_o1_r:
				file_buffer << "\t" << op_name << " ";
				opd1->print_asm_opd(file_buffer);
				file_buffer << ", ";
				result->print_asm_opd(file_buffer);
				file_buffer << "\n";
				break;

		default: CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH,
					"Intermediate code format not supported");
				break;
	}
}


Control_Flow_IC_Stmt::Control_Flow_IC_Stmt(Tgt_Op op, Ics_Opd * o1, Ics_Opd * o2, string label)
{
	CHECK_INVARIANT((machine_desc_object.spim_instruction_table[op] != NULL),
			"Instruction description in spim table cannot be null");

	op_desc = *(machine_desc_object.spim_instruction_table[op]);
	opd1 = o1;
	opd2 = o2;
	offset = label;
}

Control_Flow_IC_Stmt& Control_Flow_IC_Stmt::operator=(const Control_Flow_IC_Stmt& rhs)
{
	*this = rhs;
	return *this;
}

Ics_Opd * Control_Flow_IC_Stmt::get_opd1()          { return opd1; }
Ics_Opd * Control_Flow_IC_Stmt::get_opd2()          { return opd2; }
string Control_Flow_IC_Stmt::get_Offset()        { return offset; }

void Control_Flow_IC_Stmt::set_opd1(Ics_Opd * io)   { opd1 = io; }
void Control_Flow_IC_Stmt::set_opd2(Ics_Opd * io)   { opd2 = io; }
void Control_Flow_IC_Stmt::set_Offset(string label) { offset = label; }

void Control_Flow_IC_Stmt::print_icode(ostream & file_buffer)
{
	CHECK_INVARIANT (!offset.empty(), "offset cannot be NULL for a control IC Stmt");

	string operation_name = op_desc.get_name();

	Icode_Format ic_format = op_desc.get_ic_format();

	switch (ic_format)
	{
		case i_op_o1_o2_st:
				if(opd1 == NULL){
					file_buffer << "\tgoto " + offset;
					file_buffer <<"\n";
				}
				else{
					file_buffer << "\t" << operation_name << ":    \t";
					opd1->print_ics_opd(file_buffer);
					file_buffer << " , ";
					opd2->print_ics_opd(file_buffer);
					file_buffer << " : goto " + offset;
					file_buffer << "\n";
				}
				break;

		default: CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH,
					"Intermediate code format not supported");
				break;
	}
}

void Control_Flow_IC_Stmt::print_assembly(ostream & file_buffer)
{
	CHECK_INVARIANT (!offset.empty(), "offset cannot be NULL for a control IC Stmt");

	string op_name = op_desc.get_mnemonic();
	Assembly_Format assem_format = op_desc.get_assembly_format();

	switch (assem_format)
	{
		case a_op_o1_o2_st:
				if(opd1 == NULL){
					file_buffer << "\tj " + offset;
					file_buffer <<"\n";
				}
				else{
					file_buffer << "\t" << op_name << " ";
					opd1->print_asm_opd(file_buffer);
					file_buffer << ", ";
					opd2->print_asm_opd(file_buffer);
					file_buffer << ", " + offset;
					file_buffer << "\n";
				}
				break;

		default: CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH,
					"Intermediate code format not supported");
				break;
	}
}


Label_IC_Stmt::Label_IC_Stmt(Tgt_Op inst_op, Ics_Opd * opd1, string offset)
{
	CHECK_INVARIANT((machine_desc_object.spim_instruction_table[inst_op] != NULL),
			"Instruction description in spim table cannot be null");

	op_desc = *(machine_desc_object.spim_instruction_table[inst_op]);
	this->opd1 = opd1;
	this->offset = offset;
}

Label_IC_Stmt& Label_IC_Stmt::operator=(const Label_IC_Stmt& rhs)
{
	*this = rhs;
	return *this;
}

Ics_Opd * Label_IC_Stmt::get_opd1()          { return opd1; }
string Label_IC_Stmt::get_offset()        { return offset; }

void Label_IC_Stmt::set_opd1(Ics_Opd * io)   { opd1 = io; }
void Label_IC_Stmt::set_offset(string label) { offset = label; }

void Label_IC_Stmt::print_icode(ostream & file_buffer)
{
	CHECK_INVARIANT (!offset.empty(), "offset cannot be NULL for a label IC Stmt");

	string operation_name = op_desc.get_name();

	Icode_Format ic_format = op_desc.get_ic_format();

	switch (ic_format)
	{
	case i_op_st:
			file_buffer <<"\n" + offset + ":";
			file_buffer << "\n";
			break;

	default: CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH,
				"Intermediate code format not supported");
		break;
	}
}

void Label_IC_Stmt::print_assembly(ostream & file_buffer)
{
	CHECK_INVARIANT (!offset.empty(), "offset cannot be NULL for a label IC Stmt");

	string op_name = op_desc.get_mnemonic();
	Assembly_Format assem_format = op_desc.get_assembly_format();

	switch (assem_format)
	{
	case a_op_st:
			file_buffer <<"\n" + offset + ":";
			file_buffer << "\n";
			break;

	default: CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH,
				"Intermediate code format not supported");
		break;
	}
}

/******************************* Class Code_For_Ast ****************************/

Code_For_Ast::Code_For_Ast()
{
	//TODO
	result_register = NULL;
	ics_list.clear();
}

Code_For_Ast::Code_For_Ast(list<Icode_Stmt *> & ic_l, Register_Descriptor * reg)
{
	//TODO
	result_register = reg;
	ics_list = ic_l;
}

void Code_For_Ast::append_ics(Icode_Stmt & ic_stmt)
{
	//TODO
	ics_list.push_back(&ic_stmt);
}

list<Icode_Stmt *> & Code_For_Ast::get_icode_list()
{
	//TODO
	return ics_list;
}

Register_Descriptor * Code_For_Ast::get_reg()
{
	//TODO
	return result_register;
}

void Code_For_Ast::set_reg(Register_Descriptor * reg)
{
	//TODO
	result_register = reg;
}

Code_For_Ast& Code_For_Ast::operator=(const Code_For_Ast& rhs)
{
	//TODO
	this->result_register = rhs.result_register;
	this->ics_list = rhs.ics_list;
}

/************************ class Instruction_Descriptor ********************************/

Tgt_Op Instruction_Descriptor::get_op()                   	{ return inst_op; }
string Instruction_Descriptor::get_name()                       { return name; }
string Instruction_Descriptor::get_mnemonic()                   { return mnemonic; }
string Instruction_Descriptor::get_ic_symbol()                  { return ic_symbol; }
Icode_Format Instruction_Descriptor::get_ic_format()            { return ic_format; }
Assembly_Format Instruction_Descriptor::get_assembly_format()   { return assem_format; }

Instruction_Descriptor::Instruction_Descriptor (Tgt_Op op, string temp_name, string temp_mnemonic,
						string ic_sym, Icode_Format ic_form, Assembly_Format as_form)
{
	inst_op = op;
	name = temp_name;
	mnemonic = temp_mnemonic;
	ic_symbol = ic_sym;
	ic_format = ic_form;
	assem_format = as_form;
}

Instruction_Descriptor::Instruction_Descriptor()
{
	inst_op = nop;
	name = "";
	mnemonic = "";
	ic_symbol = "";
	ic_format = i_nsy;
	assem_format = a_nsy;
}

template class Const_Opd<int>;
template class Const_Opd<double>;
