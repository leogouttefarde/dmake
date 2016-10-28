
#include "Master.h"

extern /*readonly*/ CProxy_Master masterProxy;
extern /*readonly*/ CProxy_Slave slaveArray;
extern /*readonly*/ int nSlaves;

Slave::Slave()
{
	masterProxy.requestJob(thisIndex);
}

void Slave::run(Job &job)
{
	CkPrintf("Slave::run\n");
	File file;
	//file = job.gen()
	CkPrintf("job.data = %d\n", job.data);

	masterProxy.finishJob(file);
	masterProxy.requestJob(thisIndex);
}

