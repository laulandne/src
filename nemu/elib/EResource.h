#ifndef ELIB_ERESOURCE_H
#define ELIB_ERESOURCE_H

/*

TODO: This code has real trouble telling an actual resource file from just
any file.  Use caution when using this...
The error flag isn't used as well as it might to solve this...

*/


#include <elib/EMemAbstract.h>
#include <elib/EResObject.h>


// platform-independant resource
// this is what resources are stored as in the map
class EResource : public EResObject {
public:
  EResource(const char *tname,const char *ttype,unsigned int tnum,unsigned int toffset,unsigned int tsize);
  const char *name;
  const char *type;
  unsigned int num;
  ULONG offset;
  unsigned int size;
protected:
  void init();
};


#endif
