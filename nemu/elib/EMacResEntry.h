#ifndef ELIB_EMACRESENTRY_H
#define ELIB_EMACRESENTRY_H

/*

TODO: This code has real trouble telling an actual resource file from just
any file.  Use caution when using this...
The error flag isn't used as well as it might to solve this...

*/


//#include <alib/abaseclass.h>


// For internal use...used while reading the mac resource fork
class EMacResEntry : public EResObject {
public:
  ULONG type;
  UINT16 num;
  short refOff;
protected:
};


#endif
