#ifndef BINARYLIB_NJAVAMETHOD_H
#define BINARYLIB_NJAVAMETHOD_H


#include <elib/EMemAbstract.h>


class NJavaMethod {
public:
  ULONG addr;   // virt addr of start of struct
  unsigned int name;     // index into constants
  unsigned int flags;
  unsigned int desc;     // index into constants
  unsigned int num;      // number of attributes attached
  unsigned int attributes; // virt addr of first attribute
protected:
};


#endif
