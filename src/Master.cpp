
#include "Master.hpp"

/*readonly*/ CProxy_Master masterProxy;
/*readonly*/ CProxy_Slave slaveArray;
/*readonly*/ int nSlaves;

std::list<int> freeSlaves;

Master::Master(CkArgMsg *m)
{
	CkPrintf("Master creation with #%d processor(s)\n", CkNumPes());
	const char *target = NULL;

	masterProxy = thisProxy;
	nSlaves = CkNumPes();

	mFilesMap.resize( nSlaves );

	if (m->argc > 1) {
		target = m->argv[1];
		Node *tree = Parser::ParseFile( target, mTargets );
		bool success = false;

		if ( tree != NULL ) {
			success = tree->setDeps(mNodes, mTasks, mTargets);
		}

		if ( !success ) {
			CkExit();
		}

		CkPrintf("Fin de la construction de l'arbre\n");
		// printf("target = %s\n", target);

		mStart = std::chrono::high_resolution_clock::now();

		// chare array construction
		slaveArray = CProxy_Slave::ckNew(nSlaves);

		CkPrintf("%d slaves created\n", nSlaves);

	} else {
		// target = "Makefile";
		CkPrintf("Usage : %s <MakefilePath>\n", m->argv[0]);
		CkExit();
	}
}

void Master::exit()
{
	auto end = std::chrono::high_resolution_clock::now();

	std::cout << std::endl << "Elapsed time (ms) :" << std::endl <<
		std::chrono::duration_cast<std::chrono::milliseconds>(end-mStart).count()
		<< std::endl;

	CkExit();
}

void getMissingDeps(
	const std::vector<Node*>& deps,
	std::map<Node*,bool>& sent,
	std::vector<Node*>& oMissing)
{
	for (Node *dep : deps) {

		if (!sent.count(dep)) {
			oMissing.push_back(dep);
			sent[dep] = true;
		}
	}
}

void Master::runJobs()
{
	Node *task = NULL;

	while ( !freeSlaves.empty() && (task = nextTask()) ) {

		// Empty tasks finish instantly
		if ( task->getCmds().size() == 0 ) {
			task->setDone();
			continue;
		}

		int idx = freeSlaves.front();
		freeSlaves.pop_front();

		std::vector<Node*> deps;
		const int iNode = CkNodeOf(idx);

		// If not on master node, find all missing deps to send
		if ( iNode > 0 ) {
			getMissingDeps(task->getDeps(), mFilesMap[ idx ], deps);
		}

		Job job( task->getName(), task->getCmds(), deps );

		CkPrintf("slaveArray[%d].run\n", idx);
		slaveArray[ idx ].run( job );
		CkPrintf("slaveArray[%d].run done\n", idx);
	}

	// Exit when nothing left to do
	if (freeSlaves.size() == nSlaves) {
		this->exit();
	}
}

void Master::requestJob(int iSlave)
{
	CkPrintf("Master::requestJob\n");

	freeSlaves.push_back(iSlave);
	runJobs();
}

void Master::finishJob(int iSlave, File &target)
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

	freeSlaves.push_back(iSlave);
	runJobs();
}

Node* Master::nextTask() {
	Node *jTask = NULL;

	if ( mTasks.empty() ) {
		std::list<Node*>::iterator it;

		auto begin = std::chrono::high_resolution_clock::now();

		// Find ready task(s) from nodes
		for (it=mNodes.begin(); it!=mNodes.end(); ) {
			Node *node = *it;

			if ( node->isReady() ) {
				mTasks.push_back(node);
				it = mNodes.erase(it);

				CkPrintf("New task ready : %s\n", node->getName().c_str());
			}

			// Only increment when current
			// iterator is not consumed
			else {
				++it;
			}
		}

		auto end = std::chrono::high_resolution_clock::now();

		std::cout << "Scheduling time [ O(n) worst case ] : " << std::chrono::duration_cast<std::chrono::milliseconds>(end-begin).count() << " ms"
			<< " / " << std::chrono::duration_cast<std::chrono::nanoseconds>(end-begin).count() << " ns" << std::endl;
	}

	if ( !mTasks.empty() ) {

		jTask = mTasks.front();
		mTasks.pop_front();

		CkPrintf("\nNext task : %s\n", jTask->getName().c_str());

		// Run last job directly from master
		if ( jTask != NULL && mNodes.empty() && mTasks.empty() ) {
			Slave::ExecuteCmds( jTask->getCmds() );
			jTask = NULL;

			// Exit after last job
			this->exit();
		}
	}

	return jTask;
}


#include "Make.def.h"
