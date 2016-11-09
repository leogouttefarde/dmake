
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
			std::list<Node*>::iterator it;

			// Find ready task(s) from nodes
			for (it=mNodes.begin(); it!=mNodes.end(); ++it) {
				Node *node = *it;

				if ( node->isReady() ) {
					mTasks.push_back(node);
					it = mNodes.erase(it);
				}
			}
		}

		if ( !mTasks.empty() ) {
			jTask = mTasks.front();
			mTasks.pop_front();
		}

		return jTask;
	}


private:

	// Put leaves as first tasks as they can be built as is
	std::list<Node*> mTasks;
	std::list<Node*> mNodes;

	std::map<std::string,Node*> mTargets;

};

