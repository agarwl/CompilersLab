
#ifndef PROGRAM_HH
#define PROGRAM_HH

#include <string>
#include <map>

#define GLOB_SPACE "   "

using namespace std;

class Program;

extern Program program_object;

class Program
{
	Symbol_Table global_symbol_table;
	map<string, Procedure*> proc_map;
	list<String_Ast*> string_vars;


public:
	Program();
	~Program();
	void delete_all();

	void add_procedure(Procedure * proc, int line);
	bool procedure_in_proc_map(Procedure * proc);
	void set_global_table(Symbol_Table & new_global_table);

	Symbol_Table_Entry & get_symbol_table_entry(string variable);

	void add_string(String_Ast * string_ast);

	void print_sym();
	void print();
	void print_string_vars(ostream & file_buffer);

	bool check_proc_defined_and_called();

	bool variable_in_proc_map_check(string symbol);
	bool variable_in_symbol_list_check(string variable);
	void global_list_in_proc_check();
	bool procedure_in_proc_map(string & proc_name);
	Procedure* get_procedure(string proc_name);

	// compile
	void compile();
	void print_assembly();
};

#endif
