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

bool basicBlocks::checkInKillAndOut(Ics_Opd * opd1, Ics_Opd * opd2)
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
		cout<< "-------------Kill Set------------\n";
		for (it = visited[i]->get_kill().begin(); it != visited[i]->get_kill().end(); it++)
		{
			string com = typeid(*(*it)).name();
			if (com == "17Register_Addr_Opd")
				cout<< "\t\t" <<(*it)->get_reg()->get_name() << endl;
			else
				cout<< "\t\t" <<(*it)->get_sym_entry()->get_variable_name() << endl;
		}
		cout<< "---------------Gen Set-------------\n";
		for (it = visited[i]->get_gen().begin(); it != visited[i]->get_gen().end(); ++it)
		{
			string com = typeid(*(*it)).name();
			if (com == "17Register_Addr_Opd")
				cout<< "\t\t" << (*it)->get_reg()->get_name() << endl;
			else
				cout<< "\t\t" <<(*it)->get_sym_entry()->get_variable_name() << endl;
		}
		cout<<endl;
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
	{
		calcIn(block->get_leftBlock());
		tempOut = block->get_leftBlock()->get_in();
	}
	for (auto nextit = tempOut.begin(); nextit != tempOut.end(); ++nextit)
	{
		if (block->put_in_set(block->get_out(), (*nextit)))
			changed = true;
	}
	tempOut.clear();
	if (block->get_rightBlock() != NULL)
	{
		calcIn(block->get_rightBlock());
		tempOut = block->get_rightBlock()->get_in();
	}	
	for (auto nextit = tempOut.begin(); nextit != tempOut.end(); ++nextit)
	{
		if (block->put_in_set(block->get_out(), (*nextit)))
			changed = true;
	}
	for (auto nextit = block->get_gen().begin(); nextit != block->get_gen().end(); ++nextit)
	{
		if (block->put_in_set(block->get_in(), (*nextit)))
			changed = true;
	}
	for (auto nextit = block->get_out().begin(); nextit != block->get_out().end(); ++nextit)
	{
		pushin = true;
		for (auto anotherit = block->get_kill().begin(); anotherit != block->get_kill().end(); ++anotherit)
		{
			if (!block->checkInKillAndOut((*nextit), (*anotherit)))
				block->put_in_set(block->get_in(), (*nextit));
		}
	}
	return changed;
}
