#ifndef BINARYLIB_NJAVAATTRIBUTE_H
#define BINARYLIB_NJAVAATTRIBUTE_H


#include <elib/EMemAbstract.h>


class NJavaAttribute {
public:
  ULONG addr;   // virt addr of start of struct
  unsigned int name;     // index into constants
  unsigned int len;      // len of data
  unsigned int data;     // virt addr of start of data
protected:
};


#endif
