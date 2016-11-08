
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

	Node* nextTask() {
		Node *jTask = NULL;

		if ( mTasks.empty() ) {
			
			// Find ready task(s) from nodes
		}
		else {
			jTask = mTasks.front();
			mTasks.pop();
		}

		return jTask;
	}


private:

	// Put leaves as first tasks as they can be built as is
	std::queue<Node*> mTasks;
	std::queue<Node*> mNodes;

};

