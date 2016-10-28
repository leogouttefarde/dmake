
#pragma once
#include <string>
#include <pup_stl.h>


class Job {

public:
	int data;
	int *vals;

  Job() {
  	data = 0;
  	vals = NULL;
  }

  ~Job() {
  }

  void pup(PUP::er &p) {
  	p|data;

    if (p.isUnpacking())
      vals = new int[data];

  	// verif manuelle car data pas forcément 0 pr l'instant
    if (vals)
    PUParray(p, vals, data);
  }


private:

};


