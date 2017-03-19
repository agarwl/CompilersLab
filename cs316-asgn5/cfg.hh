
#include <string>
#include <map>
#include <set>
#include <list>

using namespace std;

#include"common-classes.hh"
#include"error-display.hh"
#include"user-options.hh"
// #include"local-environment.hh"
#include"symbol-table.hh"
#include"ast.hh"
#include"program.hh"
#include"icode.hh"

class basicBlocks
{
	list<Icode_Stmt *> bb_icode_list;
	set<Ics_Opd *> gen, in, out, kill;
	basicBlocks * leftBlock;
	basicBlocks * rightBlock;
public:
	basicBlocks();
	~basicBlocks();
	void icode_push_back(Icode_Stmt * new_stmt);
	void put_in_set(set<Ics_Opd *> temp, Ics_Opd * new_reg);
	set<Ics_Opd *> get_gen();
	set<Ics_Opd *> get_in();
	set<Ics_Opd *> get_out();
	set<Ics_Opd *> get_kill();
	void set_leftBlock(basicBlocks * block);
	void set_rightBlock(basicBlocks * block);
	basicBlocks * get_leftBlock();
	basicBlocks * get_rightBlock();
};

class cfg
{
	map<string, basicBlocks *> futureEdge;
	basicBlocks * head;
public:
	cfg();
	~cfg();
	void set_head(basicBlocks * block);
	basicBlocks * get_head();
	void add_futureEdge(basicBlocks * block, string label);	
};