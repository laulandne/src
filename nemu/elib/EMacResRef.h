#ifndef ELIB_EMACRESREF_H
#define ELIB_EMACRESREF_H

/*

TODO: This code has real trouble telling an actual resource file from just
any file.  Use caution when using this...
The error flag isn't used as well as it might to solve this...

*/


//#include <alib/abaseclass.h>


// For internal use...used while reading the mac resource fork
class EMacResRef : public EResObject {
public:
  UINT16 id;
  short nameOff;
  BYTE attrib;
  char pad0;
  long dataOff; // actually 3 bytes in file
  ULONG pad1;
protected:
};


#endif
