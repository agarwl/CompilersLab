#include "common-classes.hh"
#include "error-display.hh"
#include "user-options.hh"
#include "icode.hh"
#include "reg-alloc.hh"
#include "symbol-table.hh"
#include "ast.hh"
#include "program.hh"
#include "procedure.hh"

#define OFFSET_FOR_RA_AND_FP 8

Procedure::Procedure(Data_Type proc_return_type, string proc_name, int line)
{
	return_type = proc_return_type;
	name = proc_name;
	lineno = line;
	is_called = is_defined = false;
	sequence_ast = NULL;

};
Procedure::~Procedure()
{
	// delete sequence_ast;
};

string Procedure::get_proc_name()
{
	return name;
};
void Procedure::set_sequence_ast(Sequence_Ast & sa)
{
	sequence_ast = &sa;
};
void Procedure::set_local_list(Symbol_Table & new_list)
{
	local_symbol_table = new_list;
	local_symbol_table.set_table_scope(local);
	local_symbol_table.assign_offsets();
};

void Procedure::set_formal_list(Symbol_Table & formal_list)
{
	formal_symbol_table = formal_list;
	formal_symbol_table.set_table_scope(formal);
	formal_symbol_table.set_start_offset_of_first_symbol(OFFSET_FOR_RA_AND_FP);
	formal_symbol_table.assign_offsets();
}

Data_Type Procedure::get_return_type()
{
	return return_type;
};
Symbol_Table_Entry & Procedure::get_symbol_table_entry(string variable_name)
{
	if(variable_in_symbol_list_check(variable_name))
		return local_symbol_table.get_symbol_table_entry(variable_name);
	else if(variable_in_formal_list_check(variable_name))
		return formal_symbol_table.get_symbol_table_entry(variable_name);
};


void Procedure:: print(ostream & file_buffer)
{
	sequence_ast->print(file_buffer);
};
void Procedure:: print_sym(ostream & file_buffer)
{
	local_symbol_table.print(file_buffer);
};

bool Procedure::variable_in_symbol_list_check(string variable)
{
	return local_symbol_table.variable_in_symbol_list_check(variable);
};

bool Procedure::variable_in_formal_list_check(string variable)
{
	return formal_symbol_table.variable_in_formal_list_check(variable);
}

void Procedure::check_formal_table(Symbol_Table & table)
{
	formal_symbol_table == table;
}

void Procedure::set_is_called()
{
	is_called = true;
}

void Procedure::set_is_defined()
{
	is_defined = true;
}

bool Procedure::check_defined()
{
	return is_defined;
}

bool Procedure::check_proc_defined_and_called()
{
	return  (!is_called || is_defined);
}

string Procedure::get_variable_in_formal_list(int position)
{
	return formal_symbol_table.get_variable_in_formal_list(position);
}

Symbol_Table & Procedure::get_formal_table()
{
	return formal_symbol_table;
}
