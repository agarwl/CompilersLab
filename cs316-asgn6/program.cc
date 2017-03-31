#include "common-classes.hh"
#include "error-display.hh"
#include "user-options.hh"
#include "icode.hh"
#include "reg-alloc.hh"
#include "symbol-table.hh"
#include "ast.hh"
#include "procedure.hh"
#include "program.hh"


	// Symbol_Table global_symbol_table;
	// Procedure * procedure;


Program program_object;

Program::Program(){};
Program::~Program(){};
void Program::delete_all(){};

void Program::set_procedure(Procedure * proc, int line)
{
	// procedure = 
};
void Program::set_global_table(Symbol_Table & new_global_table)
{
	global_symbol_table = new_global_table;
};

Symbol_Table_Entry & Program::get_symbol_table_entry(string variable)
{
	return procedure->get_symbol_table_entry(variable);
};

void Program::print_sym(){};
void Program::print(){};

bool Program::variable_proc_name_check(string symbol)
{

};
bool Program::variable_in_symbol_list_check(string variable)
{
	// return  
};
void Program::global_list_in_proc_check()
{
	global_symbol_table.global_list_in_proc_map_check();
};
bool Program::variable_in_proc_map_check(string symbol){};

// compile
void Program::compile(){};
void Program::print_assembly(){};
