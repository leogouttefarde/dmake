
#pragma once
#include "Make.decl.h"
#include "Slave.h"

class Master : public CBase_Master {
 public:
  Master(CkMigrateMessage *m) {}
  Master(CkArgMsg *m);

  void requestJob(int iSlave);
  void finishJob(File &target);
};

