
#pragma once
#include "File.h"
#include "Parser.hpp"


class Job {

public:
	std::string mTarget;
	std::vector<File> mDeps;
	std::vector<std::string> mCmds;

	Job() {
	}

	Job(std::string target,
		const std::vector<std::string>& cmds,
		std::vector<Node*>& deps) {

		mTarget = target;
		mCmds = cmds;

		// Read all deps
		for (Node *dep : deps) {

			File file( dep->getName() );
			mDeps.push_back( file );
		}

	}

	~Job() {
	}

	void writeDeps() {

		for (File& file : mDeps) {
			file.write();
		}
	}

	void pup(PUP::er &p) {
		p|mTarget;
		p|mDeps;
		p|mCmds;
	}


private:

};


