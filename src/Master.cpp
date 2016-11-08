
#include "Master.hpp"
#include "Parser.hpp"

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

	std::map<std::string,Node*> map = Parser::ParseFile( target );

	Node *all = map["all"];

	if ( all != NULL ) {
		all->addDepName("hh");
		all->setDeps(map);
	}
	else {
		std::cout << "NO all ??" << std::endl;
	}

	CkExit();

	// printf("target = %s\n", target);


	CkPrintf("chare array construction\n");
	// chare array construction
	slaveArray = CProxy_Slave::ckNew(nSlaves);
	CkPrintf("slaveArray\n");
}

int n = 0;

void Master::requestJob(int iSlave)
{
	CkPrintf("Master::requestJob\n");
	// if (jobs.size() > 1)

	Job job;
	job.data = 7;
	//job = nextJob();

	if (n++ < nSlaves) {
		CkPrintf("slaveArray[%d].run\n", iSlave);
		slaveArray[iSlave].run(job);
		CkPrintf("slaveArray[%d].run done\n", iSlave);
	}
	else {
		CkExit();
	}
}

void Master::finishJob(File &target)
{
	CkPrintf("finishJob\n");

}


#include "Make.def.h"
