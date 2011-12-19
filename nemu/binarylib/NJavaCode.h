#ifndef BINARYLIB_NJAVACODE_H
#define BINARYLIB_NJAVACODE_H


#include <elib/EMemAbstract.h>


class NJavaCode {
public:
  ULONG addr;
  unsigned int name;
  unsigned int len;
  unsigned int maxStack;
  unsigned int maxLocals;
  unsigned int codeLen;
  unsigned int codeAddr;
  unsigned int exceptionLen;
  unsigned int exceptionAddr;
  unsigned int numAttrib;
  unsigned int attribAddr;
protected:
};


#endif
