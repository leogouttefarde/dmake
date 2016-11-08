
#pragma once
#include "File.h"
#include "Parser.hpp"


class Job {

public:
  std::string mTarget;
	std::vector<File> mDeps;

  Job() {
  }

  Job(Node *target) {

    mTarget = target->getName();

    // Read all deps
    for (Node *dep : target->getDeps()) {

      mDeps.push_back( File( dep->getName() ) );
    }

  }

  ~Job() {
  }

  void pup(PUP::er &p) {
    p|mTarget;
  	p|mDeps;

   //  if (p.isUnpacking())
   //    vals = new int[data];

  	// // verif manuelle car data pas forcément 0 pr l'instant
   //  if (vals)
   //  PUParray(p, vals, data);
  }


private:

};


