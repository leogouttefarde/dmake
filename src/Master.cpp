
#include "Master.hpp"

/*readonly*/ CProxy_Master masterProxy;
/*readonly*/ CProxy_Slave slaveArray;
/*readonly*/ int nSlaves;

Master::Master(CkArgMsg *m)
{
	CkPrintf("Master::Master\n");
	// init all
	masterProxy = thisProxy;
	nSlaves = 3;
	

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
