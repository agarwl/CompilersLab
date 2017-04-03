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

// compile
void Procedure:: compile()
{
	sequence_ast->compile();
};
void Procedure:: print_icode(ostream & file_buffer){};
void Procedure:: print_assembly(ostream & file_buffer)
{
	print_prologue(file_buffer);
	sequence_ast->print_assembly(file_buffer);
	print_epilogue(file_buffer);
};

void Procedure:: print_prologue(ostream & file_buffer)
{
	file_buffer << "\n\t" << ".text" << "\t\t\t" << "# The .text assembler directive indicates\n";
	file_buffer << "\t" << ".globl " << name << "\t\t" << "# The following is the code (as oppose to data)\n";
	file_buffer << name << ":\t\t\t\t" << "# .globl makes main know to the\n";
	file_buffer << "\t\t\t\t"  << "# outside of the program.\n";
	file_buffer << "# Prologue begins\n" ;
	file_buffer << "\t" << "sw $ra, 0($sp)"	<< "\t\t" << "# Save the return address\n";
	file_buffer << "\t" << "sw $fp, -4($sp)" << "\t\t" << "# Save the frame pointer\n";
	file_buffer << "\t" << "sub $fp, $sp, 8"  << "\t\t" << "# Update the frame pointer\n";
	file_buffer << "\t" << "\n";
	file_buffer << "\t" << "sub $sp, $sp, " << 8 - local_symbol_table.get_size() << "\t\t" << "# Make space for the locals\n";
	file_buffer << "# Prologue ends\n\n";

};

void Procedure:: print_epilogue(ostream & file_buffer)
{
	string label = "epilogue_" + name;
	file_buffer << "\t" << "j " << label << '\n';
	file_buffer << "\n# Epilogue Begins\n";
	file_buffer << label << ":\n";
	file_buffer << "\t" << "add $sp, $sp, " << 8 - local_symbol_table.get_size() <<  "\n";
	file_buffer << "\t" << "lw $fp, -4($sp)" << "\n";
	file_buffer << "\t" << "lw $ra, 0($sp)" << "\n";
	file_buffer << "\t" << "jr" << "\t  " << "$31" << "\t\t" << "# Jump back to the called procedure" "\n";
	file_buffer << "# Epilogue Ends\n\n";
};