
#include "Master.hpp"

extern /*readonly*/ CProxy_Master masterProxy;
extern /*readonly*/ CProxy_Slave slaveArray;
extern /*readonly*/ int nSlaves;

Slave::Slave()
{
	masterProxy.requestJob(thisIndex);
}

void Slave::ExecuteCmds(const std::vector<std::string>& cmds)
{
	// Execute each command
	for ( const std::string& cmd : cmds ) {

		system(cmd.c_str());

		std::cout << "Processor #" << CkMyPe()
			<< " executed '" << cmd << "'" << std::endl;

	}
}

void Slave::run(Job &job)
{
	CkPrintf("Slave %d from proc #%d : run %s\n", thisIndex, CkMyPe(), job.mTarget.c_str());

	// Write received dependencies
	job.writeDeps();

	// Execute target commands
	ExecuteCmds(job.mCmds);

	// Read generated target (except on master)
	File target(job.mTarget, (CkMyNode() > 0) );

	// Finish job
	masterProxy.finishJob(thisIndex, target);
}

