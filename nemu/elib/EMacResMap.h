#ifndef ELIB_EMACRESMAP_H
#define ELIB_EMACRESMAP_H

/*

TODO: This code has real trouble telling an actual resource file from just
any file.  Use caution when using this...
The error flag isn't used as well as it might to solve this...

*/


//#include <alib/abaseclass.h>


// For internal use...used while reading the mac resource fork
class EMacResMap : public EResObject {
public:
  ULONG pad0a,pad0b,pad0c,pad0d;
  ULONG pad1;
  UINT16 pad2;
  UINT16 attrib;
  UINT16 typeOff;
  UINT16 nameOff;
protected:
};


#endif
