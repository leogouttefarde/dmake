
#pragma once
#include "Make.decl.h"
#include "Slave.hpp"
#include "Parser.hpp"


class Master : public CBase_Master {

public:
	Master(CkMigrateMessage *m) {}
	Master(CkArgMsg *m);

	void requestJob(int iSlave);
	void finishJob(File &target);

	// Returns the next task to be distributed,
	// or NULL when none currently available
	Node* nextTask();


private:

	// Put leaves as first tasks as they can be built as is
	std::list<Node*> mTasks;
	std::list<Node*> mNodes;

	std::map<std::string,Node*> mTargets;

};

