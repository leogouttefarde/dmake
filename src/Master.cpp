
#include "Master.hpp"

/*readonly*/ CProxy_Master masterProxy;
/*readonly*/ CProxy_Slave slaveArray;
/*readonly*/ int nSlaves;

Master::Master(CkArgMsg *m)
{
	CkPrintf("Master creation with #%d processor(s)\n", CkNumPes());
	// init all
	const char *target = NULL;

	masterProxy = thisProxy;
	nSlaves = CkNumPes() - 1;

	if (nSlaves == 0) {
		nSlaves = 1;
	}

	if (m->argc > 1) {
		target = m->argv[1];
		Node *tree = Parser::ParseFile( target );

		if ( tree != NULL ) {
			tree->setDeps(mNodes, mTasks, *(tree->mTargets));
		}
		else {
			CkExit();
		}

		mTargets = *(tree->mTargets);

		CkPrintf("Fin de la construction de l'arbre\n");
		// CkExit();
		// printf("target = %s\n", target);
		CkPrintf("chare array construction\n");
		// chare array construction
		slaveArray = CProxy_Slave::ckNew(nSlaves);
		CkPrintf("%d slaves created\n", nSlaves);
	} else {
		// target = "Makefile";
		CkPrintf("Veuillez entrer le chemin du Makefile en argument\n");
		CkExit();
	}
}

std::list<int> freeSlaves;

void Master::requestJob(int iSlave)
{
	CkPrintf("Master::requestJob\n");
	CkPrintf("slaveArray[%d].run\n", iSlave);

	Node *task = nextTask();

	if (task != NULL) {
		Job job( task );
		slaveArray[iSlave].run( job );
	}
	else {
		freeSlaves.push_back(iSlave);

		if (freeSlaves.size() == nSlaves) {
			CkExit();
		}
	}

	CkPrintf("slaveArray[%d].run done\n", iSlave);
}

void Master::finishJob(File &target)
{
	CkPrintf("finishJob\n");

	// Ecriture du target terminé
	target.write();

	Node *node = mTargets[target.mPath];

	if (node != NULL) {
		node->setDone();
	}
	else {
		std::cout << "An unknown job just finished" << std::endl;
	}
}

Node* Master::nextTask() {
	Node *jTask = NULL;

	if ( mTasks.empty() ) {
		std::list<Node*>::iterator it;

		// Find ready task(s) from nodes
		for (it=mNodes.begin(); it!=mNodes.end(); ) {
			Node *node = *it;

			if ( node->isReady() ) {
				mTasks.push_back(node);
				it = mNodes.erase(it);
			}

			// Only increment when current
			// iterator is not consumed
			else {
				++it;
			}
		}
	}

	if ( !mTasks.empty() ) {
		jTask = mTasks.front();
		mTasks.pop_front();
	}

	return jTask;
}


#include "Make.def.h"
