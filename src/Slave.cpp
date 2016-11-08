
#include "Master.hpp"

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

	//file = job.gen()
	job.writeDeps();

    // job.mTarget

	for (std::string cmd : job.mCmds) {
		system(cmd.c_str());
		std::cout << "Executed cmd '" << cmd << "'" << std::endl;
	}

	File target(job.mTarget);
	// CkPrintf("job.data = %d\n", job.data);

	masterProxy.finishJob(target);

	// todo : autosend new job from finishJob'end instead
	masterProxy.requestJob(thisIndex);
}

