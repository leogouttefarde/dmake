/////////////////////////////////////
//
//  FilePUP.C
//
//  Definition of chares in FilePUP
//
//  Author: Eric Bohm
//  Date: 2012/7/30
//
/////////////////////////////////////

#include "FilePUP.h"

main::main(CkArgMsg *m)
{
  //normal object construction
  // File file("data.bin");
  File file("data");

  const char *data = "zcgvgczycugzebocegbvfryhihzervhniof"\
"zcgvgczycugzebocegbvfryhihzervhniofgbtrhbynnbythny"\
"zcgvgczycugzebocegbvfryhihzervhniofgbtrhbynnbythny"\
"zcgvgczycugzebocegbvfryhihzervhniofgbtrhbynnbythny"\
"zcgvgczycugzebocegbvfryhihzervhniofgbtrhbynnbythny"\
"zcgvgczycugzebocegbvfryhihzervhniofgbtrhbynnbythny"\
"zcgvgczycugzebocegbvfryhihzervhniofgbtrhbynnbythny"\
"zcgvgczycugzebocegbvfryhihzervhniofgbtrhbynnbythny"\
"zcgvgczycugzebocegbvfryhihzervhniofgbtrhbynnbythny"\
"zcgvgczycugzebocegbvfryhihzervhniofgbtrhbynnbythny";

  file.setData( (uint8_t*)data, strlen(data) );
  // file.write();

  //normal chare array construction
  CProxy_SimpleArray simpleProxy = CProxy_SimpleArray::ckNew(3);

  //pass object to remote method invocation on the chare array
  simpleProxy[2].acceptData(file);
}

#include "FilePUP.def.h"
