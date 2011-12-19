#ifndef ELIB_EMACRESHEADER_H
#define ELIB_EMACRESHEADER_H

/*

TODO: This code has real trouble telling an actual resource file from just
any file.  Use caution when using this...
The error flag isn't used as well as it might to solve this...

*/


#include <elib/EMemAbstract.h>


// For internal use...used while reading the mac resource fork
class EMacResHeader : public EResObject {
public:
  ULONG dataOff;
  ULONG mapOff;
  ULONG dataLen;
  ULONG mapLen;
protected:
};


#endif
