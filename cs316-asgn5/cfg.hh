
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
	set<Ics_Opd *> gen, in, out, kill, live;
	basicBlocks * leftBlock;
	basicBlocks * rightBlock;
public:
	basicBlocks();
	~basicBlocks();
	void icode_push_back(Icode_Stmt * new_stmt);
	bool put_in_set(set<Ics_Opd *> & temp, Ics_Opd * new_reg);
	set<Ics_Opd *> & get_gen();
	set<Ics_Opd *> & get_in();
	set<Ics_Opd *> & get_out();
	set<Ics_Opd *> & get_kill();
	set<Ics_Opd *> & get_live();
	void set_leftBlock(basicBlocks * block);
	void set_rightBlock(basicBlocks * block);
	list<Icode_Stmt *> & get_list();
	basicBlocks * get_leftBlock();
	basicBlocks * get_rightBlock();
	bool secondOp(string op);
	bool presentInSet(set<Ics_Opd *> temp, Ics_Opd * opd1);
	bool compareTwoIcsOpd(Ics_Opd * opd1, Ics_Opd * opd2);
	void createGenKill();
	bool blockKillCode();
};

class cfg
{
	map<string, basicBlocks*> labelToBlock;
	basicBlocks * head;
	list<basicBlocks *> allBlocks;
public:
	cfg();
	~cfg();
	void set_head(basicBlocks * block);
	void add_block(basicBlocks * block);
	basicBlocks * get_head();	
	basicBlocks * get_labelToBlock(string label);
	void add_labelToBlock(basicBlocks * block, string label);	
	void printCFG();
	bool calcIn(basicBlocks * block);
	void initialiseIn();
	void initLive();
	void allInOut();
	void removeDeadCode();
};