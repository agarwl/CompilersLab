#include "procedure.hh"

#include"common-classes.hh"
#include"error-display.hh"
#include"user-options.hh"
// #include"local-environment.hh"
#include"symbol-table.hh"
#include"ast.hh"
#include"procedure.hh"
#include"program.hh"
#include"icode.hh"

Sequence_Ast * Procedure::get_sequence_ast(){
	return this->sequence_ast;
}