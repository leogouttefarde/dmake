
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

      mDeps.push_back( File( dep->getName() ) );
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

   //  if (p.isUnpacking())
   //    vals = new int[data];

  	// // verif manuelle car data pas forcément 0 pr l'instant
   //  if (vals)
   //  PUParray(p, vals, data);
  }


private:

};


