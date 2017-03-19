#include <iostream>
#include <set>
#include <string>
#include <list>
#include <map>

using namespace std;


#include"common-classes.hh"
#include"error-display.hh"
#include"user-options.hh"
// #include"local-environment.hh"
#include"symbol-table.hh"
#include"ast.hh"
#include"procedure.hh"
#include"program.hh"
#include"icode.hh"

basicBlocks::basicBlocks()
{
	leftBlock = NULL;
	rightBlock = NULL;
}

basicBlocks::~basicBlocks()
{
	delete leftBlock;
	delete rightBlock;
}

void basicBlocks::icode_push_back(Icode_Stmt * new_stmt)
{
	bb_icode_list.push_back(new_stmt);
}

void basicBlocks::put_in_set(set<Ics_Opd *> temp, Ics_Opd * new_reg)
{
	temp.insert(new_reg);
	return;
}

set<Ics_Opd *> basicBlocks::get_gen()
{
	return this->gen;
}

set<Ics_Opd *> basicBlocks::get_in()
{
	return this->in;
}

set<Ics_Opd *> basicBlocks::get_out()
{
	return this->out;
}

set<Ics_Opd *> basicBlocks::get_kill()
{
	return this->kill;
}

void basicBlocks::set_leftBlock(basicBlocks * block)
{
	leftBlock = block;
}

void basicBlocks::set_rightBlock(basicBlocks * block)
{
	rightBlock = block;
}

basicBlocks * basicBlocks::get_leftBlock()
{
	return this->leftBlock;
}

basicBlocks * basicBlocks::get_rightBlock()
{
	return this->rightBlock;
}

cfg::cfg()
{
	head = NULL;
}

cfg::~cfg()
{
	delete head;
}

void cfg::set_head(basicBlocks * block)
{
	head = block;
}

basicBlocks * cfg::get_head()
{
	return this->head;
}

void cfg::add_futureEdge(basicBlocks * block, string label)
{
	futureEdge[label] = block;
}

