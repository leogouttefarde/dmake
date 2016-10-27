///////////////////////////////////////
//
//  FilePUP.h  
//
//  Declaration of chares in FilePUP
//
//  Author: Eric Bohm
//  Date: 2012/01/23
//
///////////////////////////////////////

#include "FilePUP.decl.h"

class main : public CBase_main {
 public:
  main(CkMigrateMessage *m) {}
  main(CkArgMsg *m);
};


void hostname()
{
  FILE *fp;
  char path[1035];

  /* Open the command for reading. */
  fp = popen("hostname", "r");
  if (fp == NULL) {
    printf("Failed to run command\n" );
  }

  /* Read the output a line at a time - output it. */
  while (fgets(path, sizeof(path)-1, fp) != NULL) {
    printf("%s", path);
  }

  /* close */
  pclose(fp);
}


class SimpleArray : public CBase_SimpleArray {
 public:
  SimpleArray(CkMigrateMessage *m) {}
  SimpleArray() {}

  ~SimpleArray() {}

  void done() {
    CkPrintf("Propagation terminée\n");
    //CkPrintf("Taille en octets : %d\n", localCopy.size());
    CkExit();
  }

  void acceptData(File &inFile) {
    // do something to the object
    //localCopy = inFile;

    CkPrintf("Writing %d from ", thisIndex);
    hostname();

    inFile.write( thisIndex );
    //localCopy.write( thisIndex );

    if (thisIndex == 0) { // no one lower to pass to
      done();
    }
    else { // pass object down one index
      thisProxy[thisIndex - 1].acceptData(inFile);
    }
  }

 private:
  //File localCopy;
};


