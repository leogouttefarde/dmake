
#pragma once
#include "Make.decl.h"

class Slave : public CBase_Slave {

public:
	Slave(CkMigrateMessage *m) {}
	Slave();

	~Slave() {}

	void run(Job &job);

	static void ExecuteCmds(const std::vector<std::string>& cmds);

private:

};

