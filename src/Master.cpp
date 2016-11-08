
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
	}
	else {
		target = "Makefile";
	}

	Node *tree = Parser::ParseFile( target );


	if ( tree != NULL ) {
		tree->setDeps(mNodes, mTasks);
	}
	else {
		CkExit();
	}


	// printf("target = %s\n", target);


	CkPrintf("chare array construction\n");
	// chare array construction
	slaveArray = CProxy_Slave::ckNew(nSlaves);
	CkPrintf("slaveArray\n");
}

void Master::requestJob(int iSlave)
{
	CkPrintf("Master::requestJob\n");


	CkPrintf("slaveArray[%d].run\n", iSlave);

	Node *task = nextTask();

	if (task != NULL) {
		slaveArray[iSlave].run( Job( task ) );
	}
	else {
		CkExit();
	}

	CkPrintf("slaveArray[%d].run done\n", iSlave);
}

void Master::finishJob(File &target)
{
	CkPrintf("finishJob\n");

}


#include "Make.def.h"
