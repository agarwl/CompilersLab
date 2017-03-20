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

cfg::~cfg(){}

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
	map<string, set<basicBlocks*>>::iterator it;
	it = futureEdge.find(label);
	if(it != futureEdge.end())
		it->second.insert(block);
	else
	{
		set<basicBlocks*> temp;
		temp.insert(block);
		futureEdge[label] = temp;
	}
}

map<string, set<basicBlocks*>> & cfg::get_futureEdge()
{
	return this->futureEdge;
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
	for (int i=0; i < visited.size(); i++)
	{
		cout<< visited[i] << "\tLeft:" << visited[i]->get_leftBlock() << "\tRight" << visited[i]->get_rightBlock() <<endl;
		if (visited[i]->get_leftBlock() != NULL && find(visited.begin(), visited.end(), visited[i]->get_leftBlock()) == visited.end())
			visited.push_back(visited[i]->get_leftBlock());
		if (visited[i]->get_rightBlock() != NULL && find(visited.begin(), visited.end(), visited[i]->get_rightBlock()) == visited.end())
			visited.push_back(visited[i]->get_rightBlock());
	}
}