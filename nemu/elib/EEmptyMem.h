#ifndef ELIB_EEMPTYMEM_H
#define ELIB_EEMPTYMEM_H


#include "EMemAbstract.h"


class EEmptyMem : public EMemAbstract {
public:
  EEmptyMem(unsigned int tsize);
  virtual BYTE read8(ULONG addr);
  virtual BYTE write8(ULONG addr,BYTE data);
  virtual BYTE reallyWrite8(ULONG addr,BYTE data);
protected:
};


#endif
