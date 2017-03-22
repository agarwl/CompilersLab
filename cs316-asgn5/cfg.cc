#include <iostream>
#include <set>
#include <string>
#include <list>
#include <map>
#include <queue>
#include <algorithm>

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
#include"reg-alloc.hh"

basicBlocks::basicBlocks()
{
	leftBlock = NULL;
	rightBlock = NULL;
}

basicBlocks::~basicBlocks(){}

void basicBlocks::icode_push_back(Icode_Stmt * new_stmt)
{
	bb_icode_list.push_back(new_stmt);
}

bool basicBlocks::put_in_set(set<Ics_Opd *> & temp, Ics_Opd * new_reg)
{
	set<Ics_Opd *>::iterator it;
	string com = typeid(*new_reg).name();
	if (com == "12Mem_Addr_Opd")
	{
		for (it = temp.begin(); it != temp.end(); it++)
		{
			string comanother = typeid(*(*it)).name();
			if (comanother != "12Mem_Addr_Opd")
				continue;
			if ((*it)->get_sym_entry()->get_variable_name() == new_reg->get_sym_entry()->get_variable_name())
				return false;
		}
	}
	else if (com == "17Register_Addr_Opd")
	{
		for (it = temp.begin(); it != temp.end(); it++)
		{
			string comanother = typeid(*(*it)).name();
			if (comanother !=  "17Register_Addr_Opd")
				continue;
			if ((*it)->get_reg()->get_name() == new_reg->get_reg()->get_name())
				return false;
		}
	}
	else
		return false;
	temp.insert(new_reg);
	return true;
}

set<Ics_Opd *> & basicBlocks::get_gen()
{
	return this->gen;
}

set<Ics_Opd *> & basicBlocks::get_in()
{
	return this->in;
}

set<Ics_Opd *> & basicBlocks::get_out()
{
	return this->out;
}

set<Ics_Opd *> & basicBlocks::get_kill()
{
	return this->kill;
}

set<Ics_Opd *> & basicBlocks::get_live()
{
	return this->live;
}

list<Icode_Stmt *> & basicBlocks::get_list()
{
	return this->bb_icode_list;
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

bool basicBlocks::secondOp(string op)
{
	if (op == "load" || op == "iLoad" || op == "store" || op == "load.d" || op == "iLoad.d" || op == "store.d" || op == "uminus")
		return false;
	return true;
}

bool basicBlocks::presentInKill(Ics_Opd * new_reg)
{
	set<Ics_Opd *>::iterator it;
	string com = typeid(*new_reg).name();
	if (com == "12Mem_Addr_Opd")
	{
		for (it = kill.begin(); it != kill.end(); it++)
		{
			string comanother = typeid(*(*it)).name();
			if (comanother != "12Mem_Addr_Opd")
				continue;
			if ((*it)->get_sym_entry()->get_variable_name() == new_reg->get_sym_entry()->get_variable_name())
				return true;
		}
	}
	else if (com == "17Register_Addr_Opd")
	{
		for (it = kill.begin(); it != kill.end(); it++)
		{
			string comanother = typeid(*(*it)).name();
			if (comanother !=  "17Register_Addr_Opd")
				continue;
			if ((*it)->get_reg()->get_name() == new_reg->get_reg()->get_name())
				return true;
		}
	}
	else
		return true;
	return false;
}

bool basicBlocks::compareTwoIcsOpd(Ics_Opd * opd1, Ics_Opd * opd2)
{
	string a = typeid(*opd1).name();
	string b = typeid(*opd2).name();
	if (a != b)
		return false;
	else{
		if (a == "12Mem_Addr_Opd"){
			if (opd1->get_sym_entry()->get_variable_name() == opd2->get_sym_entry()->get_variable_name())
				return true;
		}
		else if(a == "17Register_Addr_Opd"){
			if (opd1->get_reg()->get_name() == opd2->get_reg()->get_name())
				return true;
		}
	}
	return false;
}

bool basicBlocks::presentInSet(set<Ics_Opd *> temp, Ics_Opd * opd1){
	string a, b = typeid(*opd1).name();
	for (set<Ics_Opd *>::iterator it = temp.begin(); it != temp.end(); ++it)
	{
		a = typeid(*(*it)).name();
		if (a != b)
			continue;
		else{
			if (a == "12Mem_Addr_Opd"){
			if (opd1->get_sym_entry()->get_variable_name() == (*it)->get_sym_entry()->get_variable_name())
				return true;
		}
		else if(a == "17Register_Addr_Opd"){
			if (opd1->get_reg()->get_name() == (*it)->get_reg()->get_name())
				return true;
		}
		}
	}
	return false;
}

void basicBlocks::createGenKill()
{
	kill.clear();
	gen.clear();
	for ( auto it = bb_icode_list.begin(); it != bb_icode_list.end(); it++)
	{
		if ((*it)->get_op().get_name() == "beq" || (*it)->get_op().get_name() == "bne"){
			if((*it)->get_opd1() != NULL){
				if(!presentInSet(kill, (*it)->get_opd1()))
					put_in_set(gen, (*it)->get_opd1());

				if(!presentInSet(kill, (*it)->get_opd2()))	
					put_in_set(gen, (*it)->get_opd2());
			}
		}
		else{
			put_in_set(kill, (*it)->get_result());
			if(!presentInSet(kill, (*it)->get_opd1()))
				put_in_set(gen, (*it)->get_opd1());
			if(secondOp((*it)->get_op().get_name())){
				if(!presentInSet(kill, (*it)->get_opd2()))	
					put_in_set(gen, (*it)->get_opd2());
			}
			
		}
	}
}

bool basicBlocks::blockKillCode()
{
	bool changed = false;
	for(auto rit = bb_icode_list.rbegin(); rit != bb_icode_list.rend();++rit)
	{
		if ((*rit)->get_op().get_name() == "beq" || (*rit)->get_op().get_name() == "bne")
		{
			if ((*rit)->get_opd1() != NULL)
			{
				put_in_set(live, (*rit)->get_opd1());
				put_in_set(live, (*rit)->get_opd2());
			}
		}
		else{
			if (!presentInSet(live, (*rit)->get_result()))
			{
				(*rit)->print_icode(std::cout);
			//	(*it)->put_in_set((*it)->get_live(), (*rit)->get_opd1());
			//	if((*it)->secondOp((*rit)->get_op().get_name()))
			//		(*it)->put_in_set((*it)->get_live(), (*rit)->get_opd2());
				bb_icode_list.erase( next(rit).base() );
				changed = true;
			}
			else{
				put_in_set(live, (*rit)->get_opd1());
				if(secondOp((*rit)->get_op().get_name()))
					put_in_set(live, (*rit)->get_opd2());
			}
		}
	}
	return changed;
}

cfg::cfg()
{
	head = NULL;
}

cfg::~cfg(){}

void cfg::set_head(basicBlocks * block)
{
	head = block;
}

basicBlocks * cfg::get_head()
{
	return this->head;
}

void cfg::add_block(basicBlocks * block)
{
	allBlocks.push_back(block);
}

void cfg::add_labelToBlock(basicBlocks * block, string label)
{
	labelToBlock[label] = block;
}

basicBlocks * cfg::get_labelToBlock(string label)
{
	map<string, basicBlocks*>::iterator it;
	it = labelToBlock.find(label);
	if(it != labelToBlock.end())
		return it->second;	
	else
		return NULL;
}

void cfg::printCFG()
{
	vector<basicBlocks *> visited;
	visited.push_back(head);
	set<Ics_Opd *>::iterator it;
	for (int i=0; i < visited.size(); i++)
	{
		cout<<endl;
		cout<< visited[i] << "Block Number" << i << "\tLeft:" << visited[i]->get_leftBlock() << "\tRight" << visited[i]->get_rightBlock() <<endl;
		cout<< "--------------Icode List Statements-------------\n";
		for(auto it = visited[i]->get_list().begin(); it != visited[i]->get_list().end(); it++)
		{
			(*it)->print_icode(std::cout);
		}
		cout<< "Kill Set:";
		for (it = visited[i]->get_kill().begin(); it != visited[i]->get_kill().end(); it++)
		{
			string com = typeid(*(*it)).name();
			if (com == "17Register_Addr_Opd")
				cout<< "  " <<(*it)->get_reg()->get_name();
			else
				cout<< "  " <<(*it)->get_sym_entry()->get_variable_name();
		}
		cout<< "\nGen Set:";
		for (it = visited[i]->get_gen().begin(); it != visited[i]->get_gen().end(); ++it)
		{
			string com = typeid(*(*it)).name();
			if (com == "17Register_Addr_Opd")
				cout<< "  " << (*it)->get_reg()->get_name();
			else
				cout<< "  " <<(*it)->get_sym_entry()->get_variable_name();
		}
		cout<<"\nOut Set:";
		for (it = visited[i]->get_out().begin(); it != visited[i]->get_out().end(); ++it)
		{
			string com = typeid(*(*it)).name();
			if (com == "17Register_Addr_Opd")
				cout<< "  " << (*it)->get_reg()->get_name();
			else
				cout<< "  " <<(*it)->get_sym_entry()->get_variable_name();
		}
		cout<<"\nIn Set:";
		for (it = visited[i]->get_in().begin(); it != visited[i]->get_in().end(); ++it)
		{
			string com = typeid(*(*it)).name();
			if (com == "17Register_Addr_Opd")
				cout<< "  " << (*it)->get_reg()->get_name();
			else
				cout<< "  " <<(*it)->get_sym_entry()->get_variable_name();
		}
		cout<<"\nLive Set:";
		for (it = visited[i]->get_live().begin(); it != visited[i]->get_live().end(); ++it)
		{
			string com = typeid(*(*it)).name();
			if (com == "17Register_Addr_Opd")
				cout<< "  " << (*it)->get_reg()->get_name();
			else
				cout<< "  " <<(*it)->get_sym_entry()->get_variable_name();
		}
		cout<<"\n\n";
		if (visited[i]->get_leftBlock() != NULL && find(visited.begin(), visited.end(), visited[i]->get_leftBlock()) == visited.end())
			visited.push_back(visited[i]->get_leftBlock());
		if (visited[i]->get_rightBlock() != NULL && find(visited.begin(), visited.end(), visited[i]->get_rightBlock()) == visited.end())
			visited.push_back(visited[i]->get_rightBlock());
	}
}

bool cfg::calcIn(basicBlocks * block){
	bool pushin, changed = false;
	set<Ics_Opd*> tempOut;
	if (block->get_leftBlock() != NULL)
		tempOut = block->get_leftBlock()->get_in();
	for (set<Ics_Opd *>::iterator nextit = tempOut.begin(); nextit != tempOut.end(); ++nextit)
	{
		if (block->put_in_set(block->get_out(), (*nextit)))
			changed = true;
	}
	tempOut.clear();
	if (block->get_rightBlock() != NULL)
		tempOut = block->get_rightBlock()->get_in();
	for (set<Ics_Opd *>::iterator nextit = tempOut.begin(); nextit != tempOut.end(); ++nextit)
	{
		if (block->put_in_set(block->get_out(), (*nextit)))
			changed = true;
	}
	for (set<Ics_Opd *>::iterator nextit = block->get_out().begin(); nextit != block->get_out().end(); ++nextit)
	{
		pushin = true;
		for (set<Ics_Opd *>::iterator anotherit = block->get_kill().begin(); anotherit != block->get_kill().end(); ++anotherit)
		{
			if (block->compareTwoIcsOpd((*nextit), (*anotherit)))
				pushin = false;
		}
		if (pushin)
			block->put_in_set(block->get_in(), (*nextit));
	}
	return changed;
}

void cfg::initialiseIn()
{
	for (list<basicBlocks *>::iterator it = allBlocks.begin(); it != allBlocks.end(); ++it)
	{
		(*it)->get_in().clear();
		(*it)->get_out().clear();
		for (set<Ics_Opd *>::iterator nextit = (*it)->get_gen().begin(); nextit != (*it)->get_gen().end(); ++nextit)
		{
		    (*it)->put_in_set((*it)->get_in(), (*nextit));
		}
	}
}

void cfg::initLive()
{
	for (list<basicBlocks *>::iterator it = allBlocks.begin(); it != allBlocks.end(); ++it)
	{
		(*it)->get_live().clear();
		for (set<Ics_Opd *>::iterator nextit = (*it)->get_out().begin(); nextit != (*it)->get_out().end(); ++nextit)
		{
		    (*it)->put_in_set((*it)->get_live(), (*nextit));
		}
	}
}

void cfg::allInOut(){
	bool changed = calcIn(head);
	while(changed){
		changed = false;
		for (list<basicBlocks *>::iterator it = allBlocks.begin(); it != allBlocks.end(); ++it){
			bool ret = calcIn((*it));
			if (!changed)
				changed = ret;
		}
	}
}

void cfg::removeDeadCode(){
	bool changed = true;
	initLive();
	while(changed){
		changed = false;
		//printCFG();
		for (list<basicBlocks *>::iterator it = allBlocks.begin(); it != allBlocks.end(); ++it){
			
			if((*it)->blockKillCode())
			{	
				changed = true;
				(*it)->createGenKill();
			}
		}
		if (changed){
			initialiseIn();
			allInOut();
			initLive();
		}

	}
}
