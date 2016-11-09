
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

  Job(Node *target) {

    mTarget = target->getName();
    mCmds = target->getCmds();

    // Read all deps
    for (Node *dep : target->getDeps()) {

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


