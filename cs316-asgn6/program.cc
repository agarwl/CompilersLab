#include "common-classes.hh"
#include "error-display.hh"
#include "user-options.hh"
#include "icode.hh"
#include "reg-alloc.hh"
#include "symbol-table.hh"
#include "ast.hh"
#include "procedure.hh"
#include "program.hh"


Program program_object;

Program::Program(){};
Program::~Program(){};
void Program::delete_all(){};

void Program::set_procedure(Procedure * proc, int line){};
void Program::set_global_table(Symbol_Table & new_global_table){};

Symbol_Table_Entry & Program::get_symbol_table_entry(string variable){};

void Program::print_sym(){};
void Program::print(){};

bool Program::variable_proc_name_check(string symbol){};
bool Program::variable_in_symbol_list_check(string variable){};
void Program::global_list_in_proc_check(){};
bool Program::variable_in_proc_map_check(string symbol){};

// compile
void Program::compile(){};
void Program::print_assembly(){};
