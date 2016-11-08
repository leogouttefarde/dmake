
#include "Master.hpp"

/*readonly*/ CProxy_Master masterProxy;
/*readonly*/ CProxy_Slave slaveArray;
/*readonly*/ int nSlaves;

Master::Master(CkArgMsg *m)
{
	CkPrintf("Master::Master\n");
	// init all
	const char *target = NULL;

	masterProxy = thisProxy;
	nSlaves = 3;

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

		CkPrintf("Fin de la construction de l'arbre");
		// CkExit();
		// printf("target = %s\n", target);
		CkPrintf("chare array construction\n");
		// chare array construction
		slaveArray = CProxy_Slave::ckNew(nSlaves);
		CkPrintf("slaveArray\n");
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
		slaveArray[iSlave].run( Job( task ) );
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


#include "Make.def.h"
