#include "common-classes.hh"
#include "error-display.hh"
#include "user-options.hh"
#include "icode.hh"
#include "reg-alloc.hh"
#include "symbol-table.hh"
#include "ast.hh"
#include "procedure.hh"
#include "program.hh"

// #include <ostream>
// using namespace std;
// compile
void Program::compile()
{
	Procedure * main_procedure = get_procedure("main");
	for(auto it = proc_map.begin(); it != proc_map.end(); it++)
	{
		(it->second)->compile();
	}
	print_assembly();
};
void Program::print_assembly()
{
	ostream & file_buffer = command_options.get_output_buffer();
	Procedure * main_procedure = get_procedure("main");
	if (main_procedure == NULL)
		return;
	if(!global_symbol_table.is_empty() || !string_vars.empty()){
		file_buffer << "\n\t" << ".data" << "\n";
		global_symbol_table.print_assembly(file_buffer);
		print_string_vars(file_buffer);
	}
	main_procedure->print_assembly(file_buffer);
	for(auto it = proc_map.begin(); it != proc_map.end(); it++)
	{
		if(it->first != "main")
			(it->second)->print_assembly(file_buffer);
	}
};