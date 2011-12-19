#ifndef ELIB_EOPERAND_H
#define ELIB_EOPERAND_H


#include <elib/EMemAbstract.h>


// The actual use of each field is defined by the child classes
class ECPUOperand {
public:
  ECPUOperand();
  void init();
  //
  unsigned int mode;
  unsigned int size;
  unsigned int len;
  ULONG val;
  ULONG ea;
  unsigned int reg;
protected:
};


#endif
